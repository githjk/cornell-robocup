#include <string.h>
#include <iostream.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include "SampleClient.h"
#include <time.h>        
#include <sys/timeb.h>   

using namespace RoboNetwork;

SampleClient::SampleClient(char* hostname, int port, char *id) : Client(hostname, port) {
	id_ = new char[10];
	memset(id_, 0, 10);
	strncpy(id_, id, 9);
	if (connect() != 0) {
		cout << "Error connecting!" << endl;
		exit(1);
	}
	
	setSendDataSize(DATASIZE); // Allow sending 33 bytes of data
	setReceiveDataSize(DATASIZE);
	cout << ">>>>> Started connection!" << endl;
}

SampleClient::~SampleClient() {
	if (id_) delete id_;
	id_ = NULL;
}

bool SampleClient::handshake() {
	Client::send(id_, 10);
	char result[3];
	Client::receive(result, 3);
	return (0 == strcmp(result, "OK"));
}

enum { ttuUnknown, ttuHiRes, ttuClock } TimerToUse = ttuUnknown;    
LARGE_INTEGER PerfFreq;      // ticks per second                    
int PerfFreqAdjust; // in case Freq is too big                      
int OverheadTicks;   // overhead  in calling timer                  

void DunselFunction() { return; }                                       

void DetermineTimer()                                                   
{                                                                       
	void (*pFunc)() = DunselFunction;                                    
	
	// Assume the worst                                                  
	TimerToUse = ttuClock;                                               
	if ( QueryPerformanceFrequency(&PerfFreq) )                          
	{                                                                 
		// We can use hires timer, determine overhead                     
		TimerToUse = ttuHiRes;                                            
		OverheadTicks = 200;                                              
		for ( int i=0; i < 20; i++ )                                      
		{                                                              
			LARGE_INTEGER b,e;                                             
			int Ticks;                                                     
			QueryPerformanceCounter(&b);                                   
			(*pFunc)();                                                    
			
			QueryPerformanceCounter(&e);                                
			Ticks = e.LowPart - b.LowPart;                              
			if ( Ticks >= 0 && Ticks < OverheadTicks )                  
				OverheadTicks = Ticks;                                   
		}                                                           
		// See if Freq fits in 32 bits; if not lose some precision     
		PerfFreqAdjust = 0;                                            
		int High32 = PerfFreq.HighPart;                                
		while ( High32 )                                               
		{                                                           
			High32 >>= 1;                                               
			PerfFreqAdjust++;                                           
		}                                                           
	}                                                              
	return;                                                           
}




const int LOG_SIZE = 10000;

int main(int argc, char **argv) {
	char *name = argv[1];
	char *ip = "127.0.0.1";
	int port = 4444;
	
	if (argc < 2 || strlen(argv[1]) >10) {
		cerr << "ID string required (length <10 bytes) " << endl;
		name = _strdup("Zhopa0");
	}
	if (argc > 2) {
		ip = argv[2];
	}
	if (argc >3) {
		port = atoi(argv[3]);
	}
	SampleClient c = SampleClient(ip, port, name);
	char s[DATASIZE];
	cout << "Connected" << endl;
	
	/*strcpy(s, "String one");
	cout << "Sending bytes" << endl;
	int result = c.send(s);
	cout << "Result: " <<  (result == 0 ? "SUCCESS" : "ERROR!") <<endl;
	*/
	
	double time;      /* Elapsed time */             
	
	// Let any other stuff happen before we start    
	MSG msg;                                         
	PeekMessage(&msg,NULL,NULL,NULL,PM_NOREMOVE);    
	Sleep(0);                                        
	
	if ( TimerToUse == ttuUnknown )                  
		DetermineTimer();                             
	
	
	memset(s,7,DATASIZE);
	int counter = 0;
	LARGE_INTEGER start, end, frequency;
	
	LARGE_INTEGER elapsed[LOG_SIZE];
	double elapsedD[LOG_SIZE];
	QueryPerformanceFrequency(&frequency);
	while (!_kbhit() && counter < LOG_SIZE) {
		
		if ( TimerToUse == ttuHiRes )                                
		{                                                         
			LARGE_INTEGER tStart, tStop;                              
			LARGE_INTEGER Freq = PerfFreq;                            
			int Oht = OverheadTicks;                                  
			int ReduceMag = 0;                                        
			SetThreadPriority(GetCurrentThread(),                     
				THREAD_PRIORITY_TIME_CRITICAL);                        
			QueryPerformanceCounter(&tStart);             
			//QueryPerformanceCounter(&start);
			c.send(s);
			c.receive(s);
			//QueryPerformanceCounter(&end);
			//elapsed[counter].QuadPart = end.QuadPart - start.QuadPart;
			QueryPerformanceCounter(&tStop);                                
			SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);  
			// Results are 64 bits but we only do 32                        
			unsigned int High32 = tStop.HighPart - tStart.HighPart;         
			while ( High32 )                                                
			{                                                            
				High32 >>= 1;                                                
				ReduceMag++;                                                 
			}                                                            
			if ( PerfFreqAdjust || ReduceMag )                              
				
			{                                                                   
				if ( PerfFreqAdjust > ReduceMag )                                   
					ReduceMag = PerfFreqAdjust;                                      
				tStart.QuadPart = Int64ShrlMod32(tStart.QuadPart, ReduceMag);       
				tStop.QuadPart = Int64ShrlMod32(tStop.QuadPart, ReduceMag);         
				Freq.QuadPart = Int64ShrlMod32(Freq.QuadPart, ReduceMag);           
				Oht >>= ReduceMag;                                                  
			}                                                                   
			
			// Reduced numbers to 32 bits, now can do the math                     
			if ( Freq.LowPart == 0 )                                               
				time = 0.0;                                                         
			else                                                                   
				time = ((double)(tStop.LowPart - tStart.LowPart                     
				- Oht))/Freq.LowPart;                                            
		}
		elapsedD[counter] = time;		
		
		counter++;
		if (counter %1000 == 0) cout << counter << endl;
	}
	cout << "DISCONNECTING" << endl;
	cout << "Counter: " << counter <<endl;
	c.disconnect();
	
	FILE *f = fopen(name, "w");
	if (f == NULL) {
		cerr << "Failed to open log file!" << endl;
		return 1;
	} 
	for (int i = 0; i < counter - 1; i++) {
		//fprintf(f, "%I64d\n", elapsed[i]);
		fprintf(f, "%f\n", elapsedD[i]);
	}
	fclose(f);
	
	return 0;
}