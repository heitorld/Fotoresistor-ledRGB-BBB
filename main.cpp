#include "BlackGPIO/BlackGPIO.h"
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

#define PATH_ADC "/sys/bus/iio/devices/iio:device0/in_voltage"

int readAnalog(int number){
   stringstream ss;
   ss << PATH_ADC << number << "_raw";
   fstream fs;
   fs.open(ss.str().c_str(), fstream::in);
   fs >> number;
   fs.close();
   return number;
}

int main(int argc, char *argv[]){
    
	BlackLib::BlackGPIO   ledR(BlackLib::GPIO_60,BlackLib::output, BlackLib::SecureMode);
    BlackLib::BlackGPIO   ledG(BlackLib::GPIO_50,BlackLib::output, BlackLib::SecureMode);
    BlackLib::BlackGPIO   ledB(BlackLib::GPIO_51,BlackLib::output, BlackLib::SecureMode);
    BlackLib::BlackGPIO   ledV(BlackLib::GPIO_30,BlackLib::output, BlackLib::SecureMode);
    BlackLib::BlackGPIO   btn1(BlackLib::GPIO_115,BlackLib::input, BlackLib::SecureMode);

    int contador = 0;
    
    //Quando o contador chega a 15, o while é terminado e o programa também.
    while(contador <= 15){
    	//CMD1 recebe o valor numero do botão da BBB.
    	int cmd1 = btn1.getNumericValue();
    	cout << "Aperte o botão para comecar!" << endl;
    	
    	//Se o valor de CMD1 for igual a 1, ou seja, o botão for pressionado...
    	if(cmd1 == 1){
    		//Zera o contador...
    		contador = 0;
    		ledV.setValue(BlackLib::high);
    		cout << "##################" << endl;
    		cout << "PROGRAMA INICIADO!" << endl;
    		cout << "###################\n" << endl; 
    		int cmd2 = 0;
    		// E inicia outro while, agora com o programa.
    		while(cmd2 != 1){
    			// Valor recebe as informações do Fotoresistor da BBB.
    			int valor = readAnalog(1);
    			cout << "Valor: " << valor << endl;
    			
    			//Se o valor for abaixo de 1365...
    			if(valor <= 1300){
    				//O led azul é ligado.
    				ledR.setValue(BlackLib::low);
    				ledG.setValue(BlackLib::low);
    				ledB.setValue(BlackLib::high);
    				sleep(1);
    			}
    			/*else if(valor >= 1301 && valor <= 1365){
    				ledR.setValue(BlackLib::low);
    				ledG.setValue(BlackLib::high);
    				ledB.setValue(BlackLib::high);
    				sleep(1);
    			}*/
    			//Se o valor for entre 1366 e 2731...
    			else if (valor >= 1366 && valor <= 2700){
    				// O led verde é ligado
    				ledR.setValue(BlackLib::low);
    				ledG.setValue(BlackLib::high);
    				ledB.setValue(BlackLib::low);
    				sleep(1);
    			}
    			/*
    			else if(valor >= 2701 && valor <= 2731){
    				ledR.setValue(BlackLib::high);
    				ledG.setValue(BlackLib::high);
    				ledB.setValue(BlackLib::low);
    				sleep(1);
    			}*/

    			// E se o valor for entre 2732 e 4095...
    			else if(valor >= 2732 && valor <= 4095){
    				// O led vermelho é ligado
    				ledR.setValue(BlackLib::high);
    				ledG.setValue(BlackLib::low);
    				ledB.setValue(BlackLib::low);
    				sleep(1);
    			}
    			sleep(1);
    			cmd2 = btn1.getNumericValue();
    			// Se o botão for apertado novamente, o while é parado e volta pro while inicial.
    			if(cmd2 == 1){
    				cout << "##################" << endl;
    				cout << "PROGRAMA DESLIGADO!" << endl;
    				cout << "###################\n" << endl;  			
    			}	
    		}
    	}
    	sleep(1);
    	ledR.setValue(BlackLib::low);
    	ledG.setValue(BlackLib::low);
    	ledB.setValue(BlackLib::low);
    	ledV.setValue(BlackLib::low);
    	cout << "Programa fecha em: " << (15 - contador) << endl;
    	contador++;
    }	

	return 0;
}
