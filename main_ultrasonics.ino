#include <NewPing.h> 

#include <ros.h> 

#include <std_msgs/Int16MultiArray.h> 

 

// Definirea variabilelor pentru timmer 

unsigned long start_time; 

unsigned long stop_time; 

unsigned long elapsed_time; 

 

// Definirea numarului de senzori 

#define SONAR_NUM 16       // Numarul total de senzori 

#define MAX_DISTANCE 200  // Distanta maxima posibila inregistrata de senzor. 

 

// Initializarea Array-ului de senzori 

// Definirea pinilor de echo si trig pentru fiecare senzor initializat 

NewPing senzor[SONAR_NUM] = {     

  //Zona 1 - Fata                 // Setarea pinului de trig si echo, cat si distanta maxima pe care o poate citi primul senzor 

  NewPing(49, 49, MAX_DISTANCE),  // acelasi lucru dar pentru urmatorul senzor 

  NewPing(48, 48, MAX_DISTANCE), 

  NewPing(47, 47, MAX_DISTANCE), 

  NewPing(46, 46, MAX_DISTANCE), 

 

  //Zona 2 - Lateral Stanga 

  NewPing(45, 45, MAX_DISTANCE),  // acelasi lucru dar pentru urmatorul senzor 

  NewPing(44, 44, MAX_DISTANCE), 

  NewPing(43, 43, MAX_DISTANCE), 

  NewPing(42, 42, MAX_DISTANCE), 

 

  //Zona 3 - Spate 

  NewPing(41, 41, MAX_DISTANCE),  // acelasi lucru dar pentru urmatorul senzor 

  NewPing(40, 40, MAX_DISTANCE), 

  NewPing(39, 39, MAX_DISTANCE), 

  NewPing(38, 38, MAX_DISTANCE), 

 

  //Zona 4 - Lateral Dreapta 

  NewPing(37, 37, MAX_DISTANCE),  // acelasi lucru dar pentru urmatorul senzor 

  NewPing(36, 36, MAX_DISTANCE), 

  NewPing(35, 35, MAX_DISTANCE), 

  NewPing(34, 34, MAX_DISTANCE) 

}; 

 

// Definirea Arrayului in care se vor stoca distantele citite de senzori 

// Si initializarea arrayului cu valoarea 0.0 

int distante_citite[SONAR_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

 

//ROS 

//Instanțierea nodului (Node Handler care permite programului să creeze publisheri si subscriberi.  

//Node handler se ocupă, de asemenea, de comunicațiile prin portul serial. 

ros::NodeHandle  nh; 

//Definirea tipului de mesaj publicat 

std_msgs::Int16MultiArray str_msg; 

//Definirea numelui publisherului si al topicului 

ros::Publisher ultrasonic_pub("ultrasonic", &str_msg); 

 

void setup() { 

   

  //Se initializeaza nodul folosind node handler 

  //Se declara orice topic publicat 

  nh.initNode(); 

  nh.advertise(ultrasonic_pub); 

} 

 

void loop() { 

 

  // stocarea timpului cand a inceput prima iteratie 

  start_time = millis(); 

 

  // Citirea senzorilor pe rand, in functie de zona in care se afla 

  for (uint8_t i = 0; i < SONAR_NUM; i++) { 

 

    // Apelarea functiei ping pentru a returna distanta citita in cm 

    // Elementul i din array-ul distante_citite ia valoarea distantei inregistrata de senzorul i 

    distante_citite[i] = senzor[i].ping_cm(); 

    delay(40);  // Se asteapta 40ms intre citiri, ATENTIE!! valoarea minima este 29 ms, altfel citirile sunt vor fi bune 

  } 

 

  // Stocarea timpului dupa ce au fost citite datele 

  stop_time = millis(); 

 

  // Calcularea timpului petrecut pentru citirea datelor 

  elapsed_time = stop_time - start_time; 

 

  // Delay  

  delay(1000 - elapsed_time); 

  // Aici sigur au trecut 1000ms 

 

  // Dupa o secunda se transmit datele si programul trece la urmatoarea iteratie 

 

  // Publicarea datelor prin publisherul ROS 

  str_msg.data = distante_citite; 

  str_msg.data_length = SONAR_NUM; 

  ultrasonic_pub.publish( &str_msg ); 

  nh.spinOnce(); 

} 
