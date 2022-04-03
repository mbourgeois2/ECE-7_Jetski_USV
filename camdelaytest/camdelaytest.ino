//void setup() {
//  // put your setup code here, to run once:
//  pinMode(A0,INPUT);
//  Serial.begin(9600);
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//  Serial.println(analogRead(A0));
//}
const int numsamp = 50;
int i = 0;
int count[numsamp];
int sum = 0;
int mins = 1000;
int maxs = 0;

void setup()
{
  pinMode(A0, INPUT);
  //pinMode(5, INPUT);
  
  pinMode(52, OUTPUT);   // on-board LED
  Serial.begin(9600);

}

void loop()
{
     while (i<numsamp) {
     unsigned long start_time;
     unsigned long stop_time;
     
     //digitalWrite(52,0);   // Turn LED off
     //while(digitalRead(4)==0) {}
     start_time=millis();
     digitalWrite(52,1);   // Turn LED on
     while (analogRead(A0) > 1015) {}
     stop_time=millis();
     digitalWrite(52,0);
     Serial.print("time hi->lo=");
     Serial.print(stop_time-start_time);
     Serial.println(" millisec");
     count[i] = stop_time-start_time;
     if (mins > (stop_time-start_time))
        mins = stop_time-start_time;
     if (maxs < stop_time-start_time)
        maxs = stop_time-start_time;
     if (i == numsamp - 1){
        for (int j = 0; j<numsamp; j++)
        {
            sum += count[j];
        }
        Serial.print("\nfor ");
        Serial.print(numsamp);
        Serial.println(" samples:");
        Serial.print("min: ");
        Serial.print(mins);
        Serial.println(" ms");
        Serial.print("max: ");
        Serial.print(maxs);
        Serial.println(" ms");
        Serial.print("avg: ");
        Serial.print(sum / numsamp);
        Serial.println(" ms");
     }
     i++;
     delay(500);
     }
}
