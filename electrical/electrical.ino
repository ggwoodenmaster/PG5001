#define PUMP_1_CONTROL_1 22
#define PUMP_1_CONTROL_2 23
#define PUMP_1_PWM 2 // needs PWM pin
#define PUMP_2_CONTROL_1 24
#define PUMP_2_CONTROL_2 25
#define PUMP_2_PWM 3 // needs PWM pin
#define STEPPER_1_DIRECTION 26
#define STEPPER_1_STEP 27
#define STEPPER_1_STEPS_PER_REVOLUTION 100

void setup() {
    movePump(PUMP_1_CONTROL_1, PUMP_1_CONTROL_2, PUMP_1_PWM, HIGH, LOW); // pump 1 forward
    moveStepper_1(STEPPER_1_DIRECTION, STEPPER_1_STEP, HIGH); // stepper 1 to chamber 1
    movePump(PUMP_2_CONTROL_1, PUMP_2_CONTROL_2, PUMP_2_PWM, HIGH, LOW); // pump 2 forward

}

void loop() {

}

void movePump(int control_1, int control_2, int pwm, int state_1, int state_2) {
    long time_stamp = millis();
    digitalWrite(control_1, state_1);
    digitalWrite(control_2, state_2); // pump directions
    while (millis() - time_stamp < 5000) { // pump operates for 5 seconds
        analogWrite(pwm, 255); // pump full speed
    }
    analogWrite(pwm, 0); // pump stop
}

void moveStepper_1(int direction, int step, int state) {
    int current_step;
    digitalWrite(direction, state);
    for (current_step = 0; current_step < 0.5 * STEPPER_1_STEPS_PER_REVOLUTION; current_step++) {
        digitalWrite(step, HIGH); // stepper direction
        delay(5);
        digitalWrite(step, LOW);
        delay(5);
    }
}