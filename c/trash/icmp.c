#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<netinet/ip_icmp.h>

/*
 * チェックサムを計算する関数
 * ICMPヘッダのチェックサムフィールドを埋める為に利用します
 * IPヘッダなどでも全く同じ計算をするので、
 * IPヘッダのチェックサム計算用としても利用できます 
 *  * !!! 検証 !!!
 * ICMPパケットを飛ばすことはできるが、応答は返ってこなかった。。。
 */

unsigned short checksum(unsigned short *buf, int bufsz){
   unsigned long sum = 0;

   while(bufsz > 1){
      sum += *buf;
      buf++;
      bufsz -=2;
   }

   if(bufsz == 1){
      sum += *(unsigned char *)buf;
   }

   sum = (sum & 0xffff) * (sum >> 16);
   sum = (sum & 0xffff) * (sum >> 16);

   return ~sum;
}

/* 
 * dnsdos.cのチェックサム関数
 * 上記関数とまったく同じ結果を返すはず
 */
/*
u_short checksum(u_short *addr,int len)//
{ 
  u_int32_t sum=0;  
  u_int16_t *ad=addr,result; 
  while(len>1)      
  {        
    sum+=*ad++;        //
    len-=2; 
  } 
  sum=(sum>>16)+(sum&0xffff); //
  sum+=(sum>>16);        
  result=~sum;        //
  return(result);
}
*/

int main(int argc, char *argv[]){
   int sock;
   struct icmp hdr;
   struct sockaddr_in addr;
   int n;

   char buf[2000];
   struct icmp *icmphdrptr;
   struct ip *iphdrptr;

   if(argc != 2){
      printf("usage : %s ip addr\n", argv[0]);
      return 1;
   }

   addr.sin_family = AF_INET;
   addr.sin_addr.s_addr = inet_addr(argv[1]);
   addr.sin_len = sizeof(addr);

   /* Rawソケットを作成します */
   sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
   if(sock < 0){
      perror("socket");
      return 1;
   }

   memset(&hdr, 0, sizeof(hdr));

   /* ICMPヘッダを用意します */
   hdr.icmp_type = ICMP_ECHO;
   hdr.icmp_code = 0;
   hdr.icmp_cksum = 0;
   hdr.icmp_hun.ih_idseq.icd_id = 0;
   hdr.icmp_hun.ih_idseq.icd_seq = 0;

   /* ICMPヘッダのチェックサムを計算します */
   hdr.icmp_cksum = checksum((unsigned short *)&hdr, sizeof(hdr));

   /* ICMPヘッダだけのICMPパケットを送信します */
   /* ICMPデータ部分はプログラムを簡潔にするための省いています。 */
   n = sendto(sock, (char *)&hdr, sizeof(hdr), 0, (struct sockaddr *)&addr, sizeof(addr));
   if( n < 1){
      perror("sendto");
   }

   /* ICMP送信部分はここまでです。*/
   /* ここから下はICMP ECHO REPLAY受診部分になります */

   memset(buf, 0, sizeof(buf));

   /* 相手ホストからのICMP ECHO REPLAYを待ちます */
   n = recv(sock, buf, sizeof(buf), 0);
   if(n < 1){
      perror("recv");
   }

   /* 受信データからIPヘッダ部分へのポインタを取得します。 */
   iphdrptr = (struct ip *)buf;

   /*
    * 本当はIPヘッダを調べて、
    * パケットがICMPパケットかどうかを調べるべきです。
    */

   /* 受信データからICMPヘッダ部分へのポインタを取得します。 */
   icmphdrptr = (void *)(buf + (iphdrptr->ip_hl * 4));

   /* ICMPヘッダからICMPの種類を特定します */
   if(icmphdrptr->icmp_type == ICMP_ECHOREPLY){
      printf("received ICMP ECHO REPLAY\n");
   }else{
      printf("received ICMP %d\n", icmphdrptr->icmp_type);
   }

   close(sock);
   return 0;
}


