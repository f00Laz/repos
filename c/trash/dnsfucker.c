/*
 * DNS Fucker
 *
 * bakcup1 : a.a.a query ok
 * backup2 : aa.a.a query ok
 */
#include <stdio.h>
#include <stdlib.h> //for srand
#include <sys/types.h>
#include <unistd.h> //for getopt
#include <netinet/in.h> //for sockaddr_in inet_addr
#include <sys/socket.h>
#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/ip.h>
#include <string.h>

/* ip headerを自前で作成する */
struct ipheader{
   unsigned char ip_hl:4,ip_v:4;
   unsigned char ip_tos;
   unsigned short int ip_len;
   unsigned short int ip_id;
   unsigned short int ip_off;
   unsigned char ip_ttl;
   unsigned char ip_p;
   unsigned short int ip_sum;
   unsigned int ip_src;
   unsigned int ip_dst;
};

/* udp headerを自前で作成する */
struct udpheader{
   unsigned short int port_src;
   unsigned short int port_dst;
   unsigned short int udp_len;
   unsigned short int udp_sum;
};

/* dns_msgを自前で作成する */
struct dns_msg{
   struct ipheader ip;
   struct udpheader udp;
   HEADER dnshead;
   char dnsbuf[100];
};

/* checksum作成用の関数 */
u_short checksum(u_short *addr,int len)//
{ 
  u_int32_t sum=0;  
  u_int16_t *ad=addr,result; 
  while(len>1)      
  {        
    sum+=*ad++;        
    len-=2; 
  } 
  sum=(sum>>16)+(sum&0xffff); 
  sum+=(sum>>16);        
  result=~sum;        
  return(result);
}

/* ネットワークデバイスが持っているIPを取得する */
char* get_my_ipaddr(char* ndev){
   int sockfd = socket(AF_INET,SOCK_STREAM,0);
   
   struct ifreq ifr;
   ifr.ifr_addr.sa_family = AF_INET;
   strcpy(ifr.ifr_name,ndev);
   ioctl(sockfd, SIOCGIFADDR, &ifr);
   close(sockfd);

   struct sockaddr_in addr;
   memcpy( &addr, &ifr.ifr_ifru.ifru_addr, sizeof(struct sockaddr_in) );
   return inet_ntoa(addr.sin_addr);
}

/*
 * hostnameをdns queryの形式に変換する 
 * www.google.com を 3www6google3com
 */
void dns_query_format(unsigned char* dns,unsigned char* host) 
{
	int lock = 0 , i;
	strcat((char*)host,".");
	
	for(i = 0 ; i < strlen((char*)host) ; i++) 
	{
		if(host[i]=='.') 
		{
			*dns++ = i-lock;
			for(;lock<i;lock++) 
			{
				*dns++=host[lock];
			}
			lock++; //or lock=i+1;
		}
	}
	*dns++='\0';
}

int main(int argc, char **argv){
   int opts; 
   char *src_addr=(char*)calloc(16,sizeof(char));
   char *dst_addr=(char*)calloc(16,sizeof(char));
   char hostname[100];
   int sockfd;
   int ord;
   struct sockaddr_in my_addr;
   struct dns_msg dns_data;

   srand(time(NULL)); //擬似乱数の発生系列を変更する 

   //getoptsを使用して処理を振り分ける 
   while((opts=getopt(argc, argv, "s:d:t:h")) != -1){
      switch(opts){
         case 's':
            //optargを入力値をチェックすべき
            strcpy(src_addr, optarg);
            break;
         case 'd':
            //optargを入力値をチェックすべき
            strcpy(dst_addr, optarg);
            break;
         case 't':
            strcpy(hostname, optarg);
            break;
         default:
            fprintf(stderr, "Unknow option %c\n", opts);
            //usage(argv[0]);
            exit(0);
            break;
      }
   }

   //デフォルト値の設定を行う
   if(! strlen(src_addr)){
      strcpy(src_addr, get_my_ipaddr("eth0"));
   }
   if(! strlen(dst_addr)){
      strcpy(dst_addr, "192.168.1.1");
   }
   if(! strlen(hostname)){
      strcpy(hostname, "a.a.a");
   }

   //設定の最終確認を行う
   printf("set src_addr = %s\n", src_addr);
   printf("set dst_addr = %s\n", dst_addr);
   printf("set target   = %s\n", hostname);

   //Socketを作成・設定する
   if((sockfd=socket(AF_INET,SOCK_RAW,IPPROTO_RAW)) < 0){
      perror("socket error");
      exit(0);
   }
   if(setsockopt(sockfd,IPPROTO_IP,IP_HDRINCL,&ord,sizeof(ord)) < 0){
      perror("setsockopt error");
      exit(0);
   }

   bzero(&my_addr, sizeof(my_addr));
   my_addr.sin_family = AF_INET;
   my_addr.sin_port=htons(53);
   inet_aton( dst_addr, &my_addr.sin_addr );

   //DNSクエリを作成
   bzero(&dns_data, sizeof(struct dns_msg));
   //IP HEADERを埋める
   dns_data.ip.ip_v=4;
   dns_data.ip.ip_hl=(sizeof(struct ip)/4);
   dns_data.ip.ip_tos=0;
   //dns_data.ip.ip_len=sizeof(dns_data); カーネルにまかせる
   dns_data.ip.ip_id=random();
   dns_data.ip.ip_off=0;
   dns_data.ip.ip_ttl=255;
   dns_data.ip.ip_p=IPPROTO_UDP;
   dns_data.ip.ip_sum=0;
   dns_data.ip.ip_dst=my_addr.sin_addr.s_addr;
   dns_data.ip.ip_src=inet_addr(src_addr);
   //UDP HEADERを埋める
   dns_data.udp.port_dst=htons(53);
   dns_data.udp.port_src=htons(1024+(rand()%2000));
   dns_data.udp.udp_len=htons(31);//htons(sizeof(struct dns_msg)-sizeof(strut ipheader));
   dns_data.udp.udp_sum=0;
   //DNS HEADERを埋める
   dns_data.dnshead.id=random();
   //dns_data.dnshead.rd=0;
   dns_data.dnshead.rd=1;
   //dns_data.dnshead.ra=1;
   dns_data.dnshead.ra=0;
   dns_data.dnshead.aa=0;
   dns_data.dnshead.tc=0;
   //dns_data.dnshead.opcode=QUERY;
   dns_data.dnshead.opcode=0;
   dns_data.dnshead.qdcount=htons(1);
   dns_data.dnshead.ancount=htons(0);
   dns_data.dnshead.nscount=htons(0);
   dns_data.dnshead.arcount=htons(0);
   dns_data.dnshead.qr=0;
   //DNS QUERYを埋める
   /*
   dns_data.dnsbuf[0]=1;
   dns_data.dnsbuf[1]='a';
   dns_data.dnsbuf[2]=1;
   dns_data.dnsbuf[3]='a';
   dns_data.dnsbuf[4]=1;
   dns_data.dnsbuf[5]='a';
   dns_data.dnsbuf[6]=0;
   dns_data.dnsbuf[8]=1;
   dns_data.dnsbuf[10]=1;
   */
   dns_data.dnsbuf[0]=2;
   dns_data.dnsbuf[1]='a';
   dns_data.dnsbuf[2]='a';
   dns_data.dnsbuf[3]=1;
   dns_data.dnsbuf[4]='a';
   dns_data.dnsbuf[5]=1;
   dns_data.dnsbuf[6]='a';
   dns_data.dnsbuf[7]=0;
   dns_data.dnsbuf[9]=1;
   dns_data.dnsbuf[11]=1;
   dns_data.dnshead.id=random();              
   //チェックサムの計算にはUDP疑似ヘッダ(12byte)とUDPヘッダ(8byte)とUDPペイロードが必要
   //UDP疑似ヘッダの作成
   u_char *pseudo,pseudoHead[44];
   bzero(pseudoHead,44);
   pseudo=pseudoHead;
   memcpy(pseudo,&(dns_data.ip.ip_src),8);
   pseudo+=9; 
   memcpy(pseudo,&(dns_data.ip.ip_p),1);
   pseudo++;
   memcpy(pseudo,&(dns_data.udp.udp_len),2);
   pseudo+=2;
   memcpy(pseudo,&(dns_data.udp),sizeof(struct udpheader));
   pseudo+=8;
   memcpy(pseudo,&(dns_data.dnshead),sizeof(HEADER));
   pseudo+=12;
   memcpy(pseudo,&(dns_data.dnsbuf),11);//strlen(dns_data.dnsbuf));
   dns_data.udp.udp_sum=checksum((u_short *)pseudoHead,44);
   dns_data.udp.udp_sum=0;
   sendto(sockfd,&dns_data,52,0,(struct sockaddr *)&my_addr,sizeof(struct sockaddr_in));

   //確保したメモリを解放する
   free(src_addr);
   free(dst_addr);
}


