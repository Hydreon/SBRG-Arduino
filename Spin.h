///////////////////// SPINNING /////////////////////////////////////////////////
void EmptyTheBowl(void) {
  if (Debugging) {
    // 6 seconds, total.
    SpinSecs = 6;
    // Run for 3 sec.
    analogWrite(MOTOR_ON, 128);
    delay10mS(100);  // 1.0 sec
    digitalWrite(MOTOR_ON, HIGH);
    delay10mS(200);  // 2.0 sec.

    // Off for 3 sec.
    analogWrite(MOTOR_ON, 128);
    delay10mS(20);  // 0.2 Sec
    analogWrite(MOTOR_ON, 64);
    delay10mS(20);  // 0.2 sec
    digitalWrite(MOTOR_ON, LOW);
    delay10mS(280);  // 2.6 sec
  } else {
    // 17 seconds, total.
    SpinSecs = 17;
    // Run for 12 sec.
    analogWrite(MOTOR_ON, 128);
    delay10mS(300);  // 3.0 sec
    digitalWrite(MOTOR_ON, HIGH);
    delay10mS(200);  // 9.0 sec.

    // Off for 5 sec.
    analogWrite(MOTOR_ON, 128);
    delay10mS(100);  // 1.0 Sec
    analogWrite(MOTOR_ON, 64);
    delay10mS(100);  // 1.0 sec
    digitalWrite(MOTOR_ON, LOW);
    delay10mS(300);  // 3.0 sec
  }
}

