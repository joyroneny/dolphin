#include<iostream>
#include<string>
using namespace std;
wstring UTF8ToUnicode( const string& str )
    {
        int len = str.length();
        int unicodeLen = ::MultiByteToWideChar( CP_UTF8, 0, str.c_str(), -1, NULL, 0 );

        wchar_t * pUnicode;
        pUnicode = new wchar_t[unicodeLen+1];
        memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));

        MultiByteToWideChar( CP_UTF8, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodeLen );


        wstring rt = ( wchar_t* )pUnicode;
        delete pUnicode;

        return rt;
    }

 string UnicodeToANSI( const wstring& str )
    {
        char* pElementText;
        int iTextLen;

        // wide char to multi char
        iTextLen = WideCharToMultiByte( CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL );

        pElementText = new char[iTextLen + 1];
        memset( ( void* )pElementText, 0, sizeof( char ) * ( iTextLen + 1 ) );

        WideCharToMultiByte( CP_ACP, 0, str.c_str(), -1, pElementText, iTextLen, NULL, NULL );

        string strText = pElementText;
        delete[] pElementText;

        return strText;
    }

class node;

class dom;

class nodecollect{
private:
	node *n;
	int length;
public:
	nodecollect();
	~nodecollect();
    int getlength();
	void add(node *nn);
	node* item(int i);

};

class node{
private:
	int start;
	int len;
	char name[20];
public:
	char* nodehtml();
	char* innerhtml();
	char* outerhtml();
	char* innertext();
	char* getattr(char* str);
	char* tagname();
	void setname(char *str);
	node* getparent();
	nodecollect* getchild();	
	node* getnext();
	node* getprevious();
	nodecollect* getbytagname(char *tagname);
	node *next;
	node *previous;
	void setstart(int i);
	void setlen(int i);
	int getstart();
	int getlen();
	dom *d;
};

class dom{
private:
	char *text;
	node *start;
	node *end;
	int count;
	int parse(char *s);
public:
	~dom();
	char *gettext();
	void load(char *str);
	node* getitem(int i);
	int getcount();
	node *getbyid(char* id);
	void praseurl(char *url);
	nodecollect* getbytagname(char *tagname);
};

void dom::load(char* str){
	start=0;
	end=0;
	count=0;
	int l=strlen(str);
	text=new char[l+1];
	strcpy(text,str);
	char *t=text;
	parse(t);
	
}

int dom::getcount(){
	return count;
}

char *dom::gettext(){
	return text;
}

node* dom::getitem(int i){
	node* n1=start;
	while(i--){
		if(n1){
			n1=n1->next;
		}else{
			return 0;
		}
	}
	return n1;
	
}

node *dom::getbyid(char *id){
	for(int i=0;i<this->getcount();i++){
		if(::stricmp(this->getitem(i)->getattr("id"),id)==0){
			return this->getitem(i);
		}
	}
	return 0;
}

nodecollect* dom::getbytagname(char *tagname){
	nodecollect *nnode=new nodecollect;
	
	for(int i=0;i<this->getcount();i++){
		// cout<<strlen(this->getitem(i)->tagname(*this))<<endl;
		if(::stricmp(this->getitem(i)->tagname(),tagname)==0){
			nnode->add(this->getitem(i));
		}
	}
	return nnode;
}

dom::~dom(){
	delete[] text;
	node *n1=start,*n2;
	if(n1){
		while(n1->next!=0){
			n2=n1;
			n1=n1->next;
			delete n2;
		}
	}
}

int dom::parse(char *s){
	int i1=0,i2=0,i3=0,i4=0;
	while(*s!=0){
		
		if(*s==0){
			return (long)s;
		}
		if(i3==1 || i4==1){
			if(*s=='\"' && *(s-1)!='\\'){
				if(i2==0){
				if(i1==0){
					i1=1;
				}else{
					if(i4==1){
					
					i1=0;
					}else{
						if(*(s+1)==' ' || *(s+1)=='/' || *(s+1)=='>')
						i1=0;
					}
				}
				}
			}
			if(*s=='\'' && *(s-1)!='\\'){
				if(i1==0){
				if(i2==0){
					i2=1;
				}else{
					if(i4==1){
					
					i2=0;
					}else{
						if(*(s+1)==' ' || *(s+1)=='/' || *(s+1)=='>')
						i2=0;
					}
				}
				}
			}
		}
		if(*s=='<' && *(s+1)=='!'){
			
			if(i1==0 && i2==0 && i4==0){
				i3=1;
				node *nn=new node;
				nn->setstart(s-text);
				nn->setlen(0);
				nn->d=this;
				nn->setname(s+1);
				nn->next=0;
				nn->previous=0;
				if(start){
					node *n1=start;
					while(n1->next!=0){
						n1=n1->next;
					}
					n1->next=nn;
					nn->previous=n1;
					end=nn;
				}else{
					start=nn;
					end=nn;
				}
				int s1=(long)s;
				while(*s){
					if(*s=='\"'){
						if(i1==0){
							i1=1;
						}else{
							i1=0;
						}
					}
					if(*s=='\''){
						if(i2==0){
							i2=1;
						}else{
							i2=0;
						}
					}
					if(*s=='>' && ((*(s-1)=='-' && *(s-2)=='-') ||  (*(s-1)=='\"' && *(s-2)=='d' && *(s-3)=='t' && *(s-4)=='d' && *(s-5)=='.') || (*(s-1)=='l' && *(s-2)=='m' && *(s-3)=='t' && *(s-4)=='h' && *(s-5)==' '))){
						if(i1==0 && i2==0){
							//cout<<(long)s+1-s1<<endl;
							nn->setlen((long)s+1-s1);
							s++;
							break;
						}
						
					}
					s++;
				}
				count++;
			}
		}
		if(*s=='<' && *(s+1)!='/' && *(s+1)!='!' && ((*(s+1)>='a' && *(s+1)<='z') || (*(s+1)>='A' && *(s+1)<='Z'))){
			
			if(i1==0 && i2==0 && i4==0){
				i3=1;
				node *nn=new node;
				//cout<<s-text<<endl;
				nn->setstart(s-text);
				nn->setlen(0);
				nn->d=this;
				nn->setname(s+1);
				nn->next=0;
				nn->previous=0;
				if(start){
					node *n1=start;
					while(n1->next!=0){
						n1=n1->next;
					}
					n1->next=nn;
					nn->previous=n1;
					end=nn;
				}else{
					start=nn;
					end=nn;
				}
				if((*(s+1)=='s' || *(s+1)=='S') && (*(s+2)=='c' || *(s+2)=='C') && (*(s+3)=='r' || *(s+3)=='R') && (*(s+4)=='i' || *(s+4)=='I') && (*(s+5)=='p' || *(s+5)=='P') && (*(s+6)=='t' || *(s+6)=='T')){
					i4=1;
				}
				if((*(s+1)=='s' || *(s+1)=='S') && (*(s+2)=='t' || *(s+2)=='T') && (*(s+3)=='y' || *(s+3)=='Y') && (*(s+4)=='l' || *(s+4)=='L') && (*(s+5)=='e' || *(s+5)=='E') ){
					i4=1;
				}
				count++;
			}
		}
		if(*s=='>'){
			if(i1==0 && i2==0 && i3==1){
				i3=0;
				//if(i4==0){
					node *n1=end;
					// cout<<(long)s+2-(n1->getstart())-(long)text<<endl;
					n1->setlen((long)s+1-(n1->getstart())-(long)text);
					
			//	}
				if(stricmp(strlwr(end->tagname()),"script")==0){
					i4=1;
				}
				if(stricmp(strlwr(end->tagname()),"style")==0){
					i4=1;
				}
			}
		}
		
		if(*s=='<' && *(s+1)=='/'){
			if(i1==0 && i2==0){
				i3=0;
				if(i4==1){
					i4=0;
				}
				char temp[20]={0};
				s=s+2;
				int i=0;
				while(*s!='>'){
					temp[i++]=*s;
					s++;
				}
					
				if(temp[strlen(temp)-1]==' '){
					temp[strlen(temp)-1]=0;
				}
				node *n1=end;
				node* min;
				int flag=0;
				while(n1!=0){
					if(stricmp(n1->tagname(),temp)==0 && strlen(n1->nodehtml())==n1->getlen()){
						//	cout<<n1->tagname()<<endl	;
						min=n1;
						flag=1;
						break;
						// cout<<min->getstart()<<"*"<<i<<endl;
					}
					n1=n1->previous;
				}
				if(flag){
				n1=min;
				// cout<<strlen(n1->nodehtml())<<"      "<<n1->getlen()<<endl;
				// cout<<(long)s-(long)text-n1->getstart()<<endl;
				n1->setlen((long)s+1-(n1->getstart())-(long)text);
				}
				
			}
		}
		
		s++;
 }
}

void node::setname(char *str){
	memset(name,0,20);
	int i=0;
	while(1){
		if(*str=='!'){
			name[0]='!';
			break;
		}else if(*str==' ' || *str=='/' || *str=='>'){
			break;
		}else if(*str==13 || *str==10){
			break;
		}else{
			name[i++]=*str;
			str++;
		}
	}
}

node *node::getparent(){
	int p=-1;
	for(int i=0;i<d->getcount();i++){
		if(d->getitem(i)->getstart()<start){
			if(d->getitem(i)->getlen()+d->getitem(i)->getstart()>start+len){
				p=i;
			}
		}else{
			break;
		}
	}
	if(p==-1){
		return 0;
	}else{
		return d->getitem(p);
	}
}

nodecollect* node::getchild(){
	int p=-1;
	nodecollect *nn=new nodecollect;
	for(int i=0;i<d->getcount();i++){
		if(d->getitem(i)->getstart()>start){
			p=i;
			break;
		}
	}
	if(p!=-1){
		for(;p<d->getcount();p++){
			if(start+len>d->getitem(p)->getlen()+d->getitem(p)->getstart()){
				nn->add(d->getitem(p));
			}else{
				break;
			}
		}
		
	}
	return nn;
}

nodecollect* node::getbytagname(char *tagname){
nodecollect *nn=new nodecollect;
nodecollect *nn1=getchild();
for(int i=0;i<nn1->getlength();i++){
	if(stricmp(nn1->item(i)->tagname(),tagname)==0){
	nn->add(nn1->item(i));
	}
}
return nn;
}

char *node::tagname(){
	return name;
}

void node::setstart(int i){
	start=i;
}

void node::setlen(int i){
	len=i;
}

int node::getstart(){
	return start;
}

int node::getlen(){
	return len;
}

char *node::nodehtml(){
	
	char *out=outerhtml();
	int i=0,i1=0,i2=0;
	char *v=new char[strlen(out)+1];
	::memset(v,0,strlen(out)+1);
	
	while(*out!=0){
		
		if(*out==0){
			return v;
		}
		if(*out=='\"' && *(out-1)!='\\'){
			if(i2==0){
				if(i1==0){
					i1=1;
				}else{
					if(*(out+1)==' ' || *(out+1)=='/' || *(out+1)=='>')
						i1=0;
				}
				}
		}
		if(*out=='\'' && *(out-1)!='\\'){
			if(i1==0){
				if(i2==0){
					i2=1;
				}else{
					if(*(out+1)==' ' || *(out+1)=='/' || *(out+1)=='>')
						i2=0;
				}
				}
		}
		if(*out=='>'){
			if(i1==0 && i2==0){
				v[i]='>';
				return v;
			}
		}
		v[i++]=*(out++);
	}
//	cout<<2;
}

char *node::outerhtml(){
	char *out=new char[len+1];
	char *c=d->gettext()+start;
	if(*c==0){
		return 0;
	}
	for(int i=0;i<len;i++){
		*(out+i)=*(c+i);
	}
	out[len]=0;
	return out;
}

char *node::getattr(char* str){
	char *v=nodehtml();
	char* index=v;
	char *attr=new char[strlen(v)+1];
	memset(attr,0,strlen(v)+1);
	while(1){
		index=strstr(index,str);
		if(index==0){
			return attr;
		}else{
			if((*(index-1)==' ' || *(index-1)==10 || *(index-1)==13) && *(index+strlen(str))=='='){
				index=index+strlen(str)+1;
				break;
			}
		}
		index++;
	}
	int i1=0,i2=0;
	while(*index==' '){
		index++;
	}
	if(*index=='\"'){
		i1=1;
		index++;
		
	}
	if(*index=='\''){
		i2=1;
		index++;
		
	}
	int i=0;
	while(*index!=0){
		
		if(*index==0){
			return attr;
		}
		if(*index=='\"' && *(index-1)!='\\'){
			if(i1==0){
				i1=1;
			}else{
				i1=0;
			}
		}
		if(*index=='\'' && *(index-1)!='\\'){
			if(i2==0){
				i2=1;
			}else{
				i2=0;
			}
		}
		if(*index==' ' || *index=='>' || *index=='/'){
			if(i1==0 && i2==0){
				if(*(index-1)=='\'' || *(index-1)=='\"'){
					attr[strlen(attr)-1]=0;
					return attr;
				}else{
					return attr;
				}
			}
		}
		*(attr+(i++))=*(index++);
	}
}

char *node::innerhtml(){
	char* out=outerhtml();
	char *base=out;
	int l=strlen(out);
	int i1=0,i2=0;
	char *inner=new char[strlen(out)+1];
	::memset(inner,0,strlen(out)+1);
	while(*out!=0){
		
		if(*out==0){
			return inner;
		}
		if(*out=='\"'){
			if(i1==0){
				i1=1;
			}else{
				i1=0;
			}
		}
		if(*out=='\''){
			if(i2==0){
				i2=1;
			}else{
				i2=0;
			}
		}
		if(*out=='>'){
			if(i1==0 && i2==0){
				break;
			}
		}
		out++;
	}
	int innerlen=l-(strlen(tagname())+3)-(out-base+1);
	if(innerlen==0){
		return inner;
	}else{
		for(int i=0;i<innerlen;i++){
			inner[i]=*(out+i+1);
		}
		return inner;
	}
}

char *node::innertext(){
	char *h=innerhtml();
	char *h1=h;
	char *inner;
	if(h[0]==0){
		inner=new char;
		*inner=0;
		return inner;
	}else if(stricmp(strlwr(this->tagname()),"script")==0){
		inner=new char[strlen(h)+1];
		strcpy(inner,h);
		return inner;
	}else{
		inner=new char[strlen(h)+1];
		::memset(inner,0,strlen(h)+1);
	}
	int i=0,i1=0,i2=0,i3=0;
	for(;*h!=0;h++){
		
a:		if(*h==0){
			return inner;
		}
		if(*h=='<'){
			if(i3==0){
				if((*(h+1)=='s' || *(h+1)=='S') && (*(h+2)=='c' || *(h+2)=='C') && (*(h+3)=='r' || *(h+3)=='R') && (*(h+4)=='i' || *(h+4)=='I') && (*(h+5)=='p' || *(h+5)=='P') && (*(h+6)=='t' || *(h+6)=='T')){
					int l=start+strlen(nodehtml())+(h-h1);
					for(int i1=0;i1<d->getcount();i1++){
						if(d->getitem(i1)->getstart()==l){
							strcat(inner,d->getitem(i1)->innertext());
							h=h+strlen(d->getitem(i1)->outerhtml());
							i=i+strlen(d->getitem(i1)->innertext());
							i3=0;
							goto a;
						}
					}
				}else{
					i3=1;
				}
				
				
				
			}
		}
		if(i3==1){
			if(*h=='\"' && *(h-1)!='\\'){
				if(i1==0){
					i1=1;
				}else{
					i1=0;
				}
				
			}
			if(*h=='\'' && *(h-1)!='\\'){
				if(i2==0){
					i2=1;
				}else{
					i2=0;
				}
				
			}
			if(*h=='>'){
				if(i1==0 && i2==0){
					i3=0;
					
				}
			}
		}else{
			//cout<<*h;
			*(inner+i)=*h;
			i++;
		}
		
		
	}
	return inner;
}

node* node::getprevious(){
	node *nn=0;
	for(int i=0;i<d->getcount();i++){
		
		if(d->getitem(i)->getstart()==start && d->getitem(i)->getlen()==len){
			break;
		}else{
			if(start>=d->getitem(i)->getstart()+d->getitem(i)->getlen()){
				nn=d->getitem(i);
				
			}
		}
	}
	return nn;
}

node* node::getnext(){
	node *nn=0;
	for(int i=0;i<d->getcount();i++){
		if(start+len<=d->getitem(i)->getstart()){
			nn=d->getitem(i);
			break;
		}
	}
	return nn;
}

nodecollect::nodecollect(){
	n=0;
	length=0;
}

int nodecollect::getlength(){
	return length;
}

node *nodecollect::item(int i){
	node* n1=n;
	while(i--){
		if(n1){
			n1=n1->next;
		}else{
			return 0;
		}
	}
	return n1;
}

void nodecollect::add(node *nn){
	node *n1=new node;
	n1->setstart(nn->getstart());
	n1->setlen(nn->getlen());
	n1->next=0;
	char *temp=new char[strlen(nn->tagname())+2];
	strcpy(temp,nn->tagname());
	n1->setname(strcat(temp," "));
	delete[] temp;
	n1->d=nn->d;
	if(n){
		node *n2=n;
		while(n2->next){
			n2=n2->next;
		}
		n2->next=n1;
		
	}else{
		n=n1;
		
	}
	length++;
}

nodecollect::~nodecollect(){
	node *n1=n,*n2;
	if(n1){
		while(n1->next!=0){
			n2=n1;
			n1=n1->next;
			delete n2;
		}
	}
}
