extern int num;

struct flow{
	int flowID;
	struct timeval startTime;
	struct timeval endTime;
	char *prtc;
	char saddr[16], daddr[16];
	uint16_t sport, dport;
	char dir[5];
	int totPacket;
	bpf_u_int32 totBytes;
	char state[10];
	struct flow *next;
	};

struct flow *printFlow(struct flow *, int);
void printALL(struct flow *);

struct flow *addToList (int flowID, struct timeval startTime, struct timeval endTime, char *prtc, char *saddr, char *daddr,uint16_t sport, uint16_t dport, char *dir,int totPacket, bpf_u_int32 totBytes, char state[10], struct flow *head)
{
	struct flow *ptr=(struct flow *)malloc(sizeof(struct flow));
	if(head == NULL){

		ptr->flowID=flowID;
		ptr->startTime=startTime;
		ptr->endTime=endTime;
		ptr->prtc=prtc;
		strcpy(ptr->saddr,saddr);
		strcpy(ptr->daddr,daddr);
		ptr->sport=sport;
		ptr->dport=dport;
		strcpy(ptr->dir,dir);
		ptr->totPacket=1;
		ptr->totBytes=totBytes;
		strcpy(ptr->state,state);
		ptr->next=NULL;
		head=ptr;
	}
	else
	{
		ptr->flowID=flowID;
		ptr->startTime=startTime;
		ptr->endTime=endTime;
		ptr->prtc=prtc;
		strcpy(ptr->saddr,saddr);
		strcpy(ptr->daddr,daddr);
		ptr->sport=sport;
		ptr->dport=dport;
		strcpy(ptr->dir,dir);
		ptr->totPacket=1;
		ptr->totBytes=totBytes;
		strcpy(ptr->state,state);
		ptr->next=head;
		head=ptr;
	}
	return(head);
}

void print(struct flow *head)
{
	struct flow *ptr;
	ptr=head;
	while(ptr!=NULL)
	{
		printf("%d ", ptr->flowID);
		ptr=ptr->next;
	}
}

struct flow *checkIfNewFlow (struct timeval pcktime, char *prtc, char saddr[16], char daddr[16], uint16_t sport,uint16_t  dport, char dir[5], bpf_u_int32 bytes, char state[10], struct flow *head)
{
	static int flowID=0;
	struct flow *temp,*prev;
	temp=head;
	flowID++;

	if(flowID==1)
	{
		head=addToList(flowID, pcktime, pcktime, prtc, saddr, daddr, sport, dport, dir,1, bytes, state, head);
		if(!(strcmp(state,"RST"))){
			strcpy(temp->state,"RST");
			head=printFlow(head,head->flowID);
		}
		else if ((!(strcmp(state,"FINACK"))))
		{
			strcpy(temp->state,"FIN2");
		}
		else if ((!(strcmp(state,"FIN"))))
		{
			strcpy(temp->state,"FIN1");
		}
		return(head);
	}
	else
	{
		while(temp!=NULL)
		{
			if(!(strcmp(temp->saddr, saddr)) && !(strcmp(temp->daddr, daddr)) && temp->sport==sport && temp->dport==dport)
			{
				temp->endTime=pcktime;
				temp->totPacket++;
				temp->totBytes=temp->totBytes+bytes;
				if((!(strcmp(state,"ACK"))) && (!(strcmp(temp->state,"FIN3")))){
					strcpy(temp->state,"FIN");
					head=printFlow(head,temp->flowID);
				}
				else if ((!(strcmp(state,"RST"))))
				{
					strcpy(temp->state,"RST");
					head=printFlow(head,temp->flowID);
				}
				else if ((!(strcmp(state,"FINACK"))) && (!(strcmp(temp->state,"FIN2"))))
				{
					strcpy(temp->state,"FIN3");
				}
				else if ((!(strcmp(state,"FINACK"))))
				{
					strcpy(temp->state,"FIN2");
				}
				else if ((!(strcmp(state,"FIN"))))
				{
					strcpy(temp->state,"FIN1");
				}
				else if(!(strcmp(state,"SYN")))
				{
					strcpy(temp->state,state);
				}
				else if(!(strcmp(state,"SYNASK")))
				{
					strcpy(temp->state,state);
				}
				else if(!(strcmp(state,"RST")))
				{
					strcpy(temp->state,state);
				}
				else if(!(strcmp(state,"EST")))
				{
					strcpy(temp->state,state);
				}
				else if(!(strcmp(state,"ACK")))
				{
					strcpy(temp->state,"EST");
				}
				else if(!(strcmp(state,"ECORLY")))
				{
					strcpy(temp->state,"ECORLY");
				}
				else if(!(strcmp(state,"UNRCH")))
				{
					strcpy(temp->state,"UNRCH");
				}
				else if(!(strcmp(state,"SRCQUE")))
				{
					strcpy(temp->state,"SRCQUE");
				}
				else if(!(strcmp(state,"REDIRECT")))
				{
					strcpy(temp->state,"REDIRECT");
				}
				else if(!(strcmp(temp->state,"ECHO")))
				{
					strcpy(state,"ECHO");
				}
				else if(!(strcmp(state,"TIMEXD")))
				{
					strcpy(temp->state,"TIMEXD");
				}
				else if(!(strcmp(state,"PARPBLM")))
				{
					strcpy(temp->state,"PARPBLM");
				}
				else if(!(strcmp(state,"TSTMREQ")))
				{
					strcpy(temp->state,"TSTMREQ");
				}
				else if(!(strcmp(state,"TSTMRLY")))
				{
					strcpy(temp->state,"TSTMRLY");
				}
				else if(!(strcmp(state,"REQINFO")))
				{
					strcpy(temp->state,"REQINFO");
				}
				else if(!(strcmp(state,"INFORLY")))
				{
					strcpy(temp->state,"INFORLY");
				}
				else if(!(strcmp(state,"ADRMREQ")))
				{
					strcpy(temp->state,"ADRMREQ");
				}
				else if(!(strcmp(state,"ADRMRLY")))
				{
					strcpy(temp->state,"ADRMRLY");
				}

				return(head);
			}

			else if(!(strcmp(temp->saddr, daddr)) && !(strcmp(temp->daddr, saddr)) && temp->sport==dport && temp->dport==sport)
			{
				temp->endTime=pcktime;
				temp->totPacket++;
				temp->totBytes=temp->totBytes+bytes;
				if((!(strcmp(state,"ACK"))) && (!(strcmp(temp->state,"FIN3")))){
					strcpy(temp->state,"FIN");
					head=printFlow(head,temp->flowID);
				}
				else if ((!(strcmp(state,"RST"))))
				{
					strcpy(temp->state,"RST");
					head=printFlow(head,temp->flowID);
				}
				else if ((!(strcmp(state,"FINACK"))) && (!(strcmp(temp->state,"FIN2"))))
				{
					strcpy(temp->state,"FIN3");
				}
				else if ((!(strcmp(state,"FINACK"))))
				{
					strcpy(temp->state,"FIN2");
				}
				else if ((!(strcmp(state,"FIN"))))
				{
					strcpy(temp->state,"FIN1");
				}
				else if(!(strcmp(state,"SYN")))
				{
					strcpy(temp->state,state);
				}
				else if(!(strcmp(state,"SYNASK")))
				{
					strcpy(temp->state,state);
				}
				else if(!(strcmp(state,"RST")))
				{
					strcpy(temp->state,state);
				}
				else if(!(strcmp(state,"EST")))
				{
					strcpy(temp->state,state);
				}
				else if(!(strcmp(state,"ACK")))
				{
					strcpy(temp->state,"EST");
				}
				else if(!(strcmp(prtc,"UDP")))
				{
					strcpy(temp->dir,"<->");
				}
				else if(!(strcmp(state,"ECORLY")))
				{
					strcpy(temp->state,"ECORLY");
					strcpy(temp->dir,"<->");
				}
				else if(!(strcmp(state,"UNRCH")))
				{
					strcpy(temp->state,"UNRCH");
					strcpy(temp->dir,"<->");
				}
				else if(!(strcmp(state,"SRCQUE")))
				{
					strcpy(temp->state,"SRCQUE");
					strcpy(temp->dir,"<->");
				}
				else if(!(strcmp(state,"REDIRECT")))
				{
					strcpy(temp->state,"REDIRECT");
					strcpy(temp->dir,"<->");
				}
				else if(!(strcmp(temp->state,"ECHO")))
				{
					strcpy(state,"ECHO");
					strcpy(temp->dir,"<->");
				}
				else if(!(strcmp(state,"TIMEXD")))
				{
					strcpy(temp->state,"TIMEXD");
					strcpy(temp->dir,"<->");
				}
				else if(!(strcmp(state,"PARPBLM")))
				{
					strcpy(temp->state,"PARPBLM");
					strcpy(temp->dir,"<->");
				}
				else if(!(strcmp(state,"TSTMREQ")))
				{
					strcpy(temp->state,"TSTMREQ");
					strcpy(temp->dir,"<->");
				}
				else if(!(strcmp(state,"TSTMRLY")))
				{
					strcpy(temp->state,"TSTMRLY");
					strcpy(temp->dir,"<->");
				}
				else if(!(strcmp(state,"REQINFO")))
				{
					strcpy(temp->state,"REQINFO");
					strcpy(temp->dir,"<->");
				}
				else if(!(strcmp(state,"INFORLY")))
				{
					strcpy(temp->state,"INFORLY");
					strcpy(temp->dir,"<->");
				}
				else if(!(strcmp(state,"ADRMREQ")))
				{
					strcpy(temp->state,"ADRMREQ");
					strcpy(temp->dir,"<->");
				}
				else if(!(strcmp(state,"ADRMRLY")))
				{
					strcpy(temp->state,"ADRMRLY");
					strcpy(temp->dir,"<->");
				}

				return(head);
			}

			temp=temp->next;
		}

//		NEW flow

		head=addToList(flowID, pcktime,pcktime, prtc, saddr, daddr, sport, dport, dir,1, bytes, state, head);
		if(!(strcmp(state,"FIN"))){
		head=printFlow(head,head->flowID);
		}
	}
	return(head);
}

/* Note, this routine returns a pointer into a static buffer, and
 * so each call overwrites the value returned by the previous call.
 */

const char *timestamp_string(struct timeval ts)
	{
	static char timestamp_string_buf[256];
	struct tm * ptm;

	ptm = localtime ( &ts.tv_sec );
	sprintf(timestamp_string_buf, "%2d:%2d:%2d.%06d",(ptm->tm_hour)%24, ptm->tm_min, ptm->tm_sec, (int) ts.tv_usec);
	return timestamp_string_buf;
	}


struct flow *printFlow(struct flow *head, int flowID)
{
	struct flow *prev, *cur;
	struct timeval diff;
	int sec;
	prev=head;
	cur=head;
	char sport[15],dport[15];
	static int count=0;

	if(num>0 && count==num)
	{
		printf("\n");
		exit(0);
	}
	count++;
	while(cur->flowID!=flowID)
	{
		prev=cur; cur=cur->next;
		if(cur==NULL){printf("Sorry wrong entry in linked list"); return(head);}
	}

	diff.tv_sec=cur->endTime.tv_sec - cur->startTime.tv_sec;
	diff.tv_usec=cur->endTime.tv_usec - cur->startTime.tv_usec;
	sec=(int) diff.tv_usec;
	if(sec<0){sec=1-sec;}
	sprintf(sport, "%d",cur->sport);
	sprintf(dport,"%d",cur->dport);
	if(!(strcmp(cur->prtc, "ICMP"))){strcpy(sport,""); strcpy(dport,"");}

	printf("\n%s \t%s \t%s \t%s \t%s \t%s \t\t%s \t%d \t%d \t\t%s \t%d.%06d", timestamp_string(cur->startTime), cur->prtc, cur->saddr, sport, cur->dir, cur->daddr, dport, cur->totPacket, cur->totBytes, cur->state, (int) diff.tv_sec, sec);

	if(cur==head)
	{
		return(cur->next);
	}
	else
	{
		prev->next=cur->next;
		return(head);
	}
	free(cur);
	return(head);
}


void printALL(struct flow *head)
{
	struct flow *temp;
	temp=head;
	while(temp!=NULL)
	{
		temp=printFlow(temp, temp->flowID);
	}
}

struct flow * ifTimedOut(struct flow *head, int sec, struct timeval curtime)
{
	struct flow *temp;
	temp=head;
	struct timeval diff;

	if(temp==NULL || sec==0){return(head);}

	while(temp!=NULL ){
		diff.tv_sec=curtime.tv_sec - temp->startTime.tv_sec;
		if((int)diff.tv_sec > sec)
		{
			head=printFlow(head, temp->flowID);
		}
		if((int)diff.tv_sec == sec)
		{
			diff.tv_usec= curtime.tv_usec -temp->startTime.tv_usec;
			if((int)diff.tv_usec>0)
			{
				head=printFlow(head, temp->flowID);
			}
		}

		temp=temp->next;

	}
	return(head);
}
