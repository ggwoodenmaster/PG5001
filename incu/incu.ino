#define CHAMBER_TWO_IN_TIME 2
#define FRESH_MEDIA_IN_TIME_UPON_CHAMBER_TWO 2
#define CARBON_DIOXIDE_TO_TOTAL_PERCENTAGE 2
#define TOTAL_GAS_IN_TIME_UPON_CHAMBER_TWO 2
#define CARBON_DIOXIDE_IN_TIME_UPON_CHAMBER_TWO \
        TOTAL_GAS_IN_TIME_UPON_CHAMBER_TWO * CARBON_DIOXIDE_TO_TOTAL_PERCENTAGE
#define NITROGEN_IN_TIME_UPON_CHAMBER_TWO \
        TOTAL_GAS_IN_TIME_UPON_CHAMBER_TWO * (1 - CARBON_DIOXIDE_TO_TOTAL_PERCENTAGE)
#define CHECKPOINT_INTERVAL_TOTAL_SIZE 11
#define CHECKPOINT_INTERVAL_BEFORE_PERFUSION_SIZE 7
#define CHECKPOINT_INTERVAL {1,2,3,4,5,6,7,8,9,10,11}
#define PERFUSION_FACTOR_SIZE 4
#define PERFUSION_FACTOR {0.35,0.45,0.55,0.65}
#define WASTE_MEDIA_OUT_TIME_BEFORE_PERFUSION 2 // main gas out + media loss
#define TOTAL_GAS_IN_TIME_BEFORE_PERFUSION 2
#define FRESH_MEDIA_IN_TIME_BEFORE_PERFUSION \
        2 + 0.02 * WASTE_MEDIA_OUT_TIME_BEFORE_PERFUSION // 57 mL + media loss
#define CARBON_DIOXIDE_IN_TIME_BEFORE_PERFUSION \
        TOTAL_GAS_IN_TIME_BEFORE_PERFUSION * CARBON_DIOXIDE_TO_TOTAL_PERCENTAGE
#define NITROGEN_IN_TIME_BEFORE_PERFUSION \
        TOTAL_GAS_IN_TIME_BEFORE_PERFUSION * (1 - CARBON_DIOXIDE_TO_TOTAL_PERCENTAGE)
#define TOTAL_MEDIA_OUT_TIME_PERFUSION 2 // main media out + main gas out
#define AIR_OUT_PERCENTAGE_PERFUSION 0.02

// Chamber 2: ONE-A; fresh media: ONE-B; waste: TWO-A; CO2: TWO-B; nitrogen: THREE-A
#define MOTOR_CHIP_ONE_PWMA_A 22
#define MOTOR_CHIP_ONE_PWMA_B 23
#define MOTOR_CHIP_ONE_INPUT_A_ONE 24
#define MOTOR_CHIP_ONE_INPUT_A_TWO 25
#define MOTOR_CHIP_ONE_INPUT_B_ONE 26
#define MOTOR_CHIP_ONE_INPUT_B_TWO 27
#define MOTOR_CHIP_TWO_PWMA_A 28
#define MOTOR_CHIP_TWO_PWMA_B 29
#define MOTOR_CHIP_TWO_INPUT_A_ONE 30
#define MOTOR_CHIP_TWO_INPUT_A_TWO 31
#define MOTOR_CHIP_TWO_INPUT_B_ONE 32
#define MOTOR_CHIP_TWO_INPUT_B_TWO 33
#define MOTOR_CHIP_THREE_PWMA_A 34
#define MOTOR_CHIP_THREE_PWMA_B 35
#define MOTOR_CHIP_THREE_INPUT_A_ONE 36
#define MOTOR_CHIP_THREE_INPUT_A_TWO 37
#define MOTOR_CHIP_THREE_INPUT_B_ONE 38
#define MOTOR_CHIP_THREE_INPUT_B_TWO 39
#define NITROGEN_RELAY 40
#define CARBON_DIOXIDE_RELAY 41

void setup() {
        pinMode(MOTOR_CHIP_ONE_PWMA_A, OUTPUT);
        pinMode(MOTOR_CHIP_ONE_PWMA_B, OUTPUT);
        pinMode(MOTOR_CHIP_ONE_INPUT_A_ONE, OUTPUT);
        pinMode(MOTOR_CHIP_ONE_INPUT_A_TWO, OUTPUT);
        pinMode(MOTOR_CHIP_ONE_INPUT_B_ONE, OUTPUT);
        pinMode(MOTOR_CHIP_ONE_INPUT_B_TWO, OUTPUT);
        pinMode(MOTOR_CHIP_TWO_PWMA_A, OUTPUT);
        pinMode(MOTOR_CHIP_TWO_PWMA_B, OUTPUT);
        pinMode(MOTOR_CHIP_TWO_INPUT_A_ONE, OUTPUT);
        pinMode(MOTOR_CHIP_TWO_INPUT_A_TWO, OUTPUT);
        pinMode(MOTOR_CHIP_TWO_INPUT_B_ONE, OUTPUT);
        pinMode(MOTOR_CHIP_TWO_INPUT_B_TWO, OUTPUT);
        pinMode(MOTOR_CHIP_THREE_PWMA_A, OUTPUT);
        pinMode(MOTOR_CHIP_THREE_PWMA_B, OUTPUT);
        pinMode(MOTOR_CHIP_THREE_INPUT_A_ONE, OUTPUT);
        pinMode(MOTOR_CHIP_THREE_INPUT_A_TWO, OUTPUT);
        pinMode(MOTOR_CHIP_THREE_INPUT_B_ONE, OUTPUT);
        pinMode(MOTOR_CHIP_THREE_INPUT_B_TWO, OUTPUT);
        pinMode(NITROGEN_RELAY, OUTPUT);
        pinMode(CARBON_DIOXIDE_RELAY, OUTPUT);
        setMotorchipdirection(MOTOR_CHIP_ONE_INPUT_A_ONE, MOTOR_CHIP_ONE_INPUT_A_TWO); // Chamber 2 clockwise
        setMotorchipdirection(MOTOR_CHIP_ONE_INPUT_B_ONE, MOTOR_CHIP_ONE_INPUT_B_TWO); // Fresh media clockwise
        setMotorchipdirection(MOTOR_CHIP_TWO_INPUT_A_ONE, MOTOR_CHIP_TWO_INPUT_A_TWO); // Waste clockwise
        long time = millis();
        int checkpoint_interval_index = 0;
        int currentday = getCheckpoint(checkpoint_interval_index);
        int perfusion_index = 0;
        analogWrite(MOTOR_CHIP_ONE_PWMA_A, 255); // Chamber 2 full
        while (millis() - time < CHAMBER_TWO_IN_TIME) {};
        analogWrite(MOTOR_CHIP_ONE_PWMA_A, 0); // Chamber 2 stop
        time = millis();
        analogWrite(MOTOR_CHIP_ONE_PWMA_B, 255); // Fresh media full
        while (millis() - time < FRESH_MEDIA_IN_TIME_UPON_CHAMBER_TWO) {};
        analogWrite(MOTOR_CHIP_ONE_PWMA_B, 0); // Fresh media stop
        time = millis();
        digitalWrite(CARBON_DIOXIDE_RELAY, HIGH); // HIGH means closed-circuit
        setvalvedirection(MOTOR_CHIP_TWO_INPUT_B_ONE, MOTOR_CHIP_TWO_INPUT_B_TWO, true);
        analogWrite(MOTOR_CHIP_TWO_PWMA_B, 255); // CO2 open
        delay(5000);
        digitalWrite(CARBON_DIOXIDE_RELAY, LOW);
        analogWrite(MOTOR_CHIP_TWO_PWMA_B, 0);
        while (millis() - time < CARBON_DIOXIDE_IN_TIME_UPON_CHAMBER_TWO) {};
        digitalWrite(CARBON_DIOXIDE_RELAY, HIGH); // HIGH means closed-circuit
        setvalvedirection(MOTOR_CHIP_TWO_INPUT_B_ONE, MOTOR_CHIP_TWO_INPUT_B_TWO, false);
        analogWrite(MOTOR_CHIP_TWO_PWMA_B, 255); // CO2 close
        delay(5000);
        digitalWrite(CARBON_DIOXIDE_RELAY, LOW);
        analogWrite(MOTOR_CHIP_TWO_PWMA_B, 0);
        time = millis();
        digitalWrite(NITROGEN_RELAY, HIGH); // HIGH means closed-circuit
        setvalvedirection(MOTOR_CHIP_THREE_INPUT_A_ONE, MOTOR_CHIP_THREE_INPUT_A_TWO, true);
        analogWrite(MOTOR_CHIP_THREE_PWMA_A, 255); // N2 open
        delay(5000);
        digitalWrite(NITROGEN_RELAY, LOW);
        while (millis() - time < NITROGEN_IN_TIME_UPON_CHAMBER_TWO) {};
        setvalvedirection(MOTOR_CHIP_THREE_INPUT_A_ONE, MOTOR_CHIP_THREE_INPUT_A_TWO, false);
        analogWrite(MOTOR_CHIP_THREE_PWMA_A, 255); // N2 close
        delay(5000);
        digitalWrite(NITROGEN_RELAY, LOW);
        analogWrite(MOTOR_CHIP_THREE_PWMA_A, 0);
        while (currentday <= CHECKPOINT_INTERVAL_BEFORE_PERFUSION_SIZE) {
                time = millis();
                while (millis() - time < 86400000) {};
                time = millis();
                checkpoint_interval_index += 1;
                currentday = getCheckpoint(checkpoint_interval_index);
                analogWrite(MOTOR_CHIP_TWO_PWMA_A, 255); // Waste full
                while (millis() - time < WASTE_MEDIA_OUT_TIME_BEFORE_PERFUSION) {};
                analogWrite(MOTOR_CHIP_TWO_PWMA_A, 0); // Waste stop
                analogWrite(MOTOR_CHIP_TWO_PWMA_A, 255); 
                time = millis();
                analogWrite(MOTOR_CHIP_ONE_PWMA_B, 255); // Fresh media full
                while (millis() - time < FRESH_MEDIA_IN_TIME_BEFORE_PERFUSION) {};
                analogWrite(MOTOR_CHIP_ONE_INPUT_B_ONE, 0); // Fresh media stop
                time = millis();
                digitalWrite(CARBON_DIOXIDE_RELAY, HIGH); // HIGH means closed-circuit
                setvalvedirection(MOTOR_CHIP_TWO_INPUT_B_ONE, MOTOR_CHIP_TWO_INPUT_B_TWO, true);
                analogWrite(MOTOR_CHIP_TWO_PWMA_B, 255); // CO2 open
                delay(5000);
                digitalWrite(CARBON_DIOXIDE_RELAY, LOW);
                analogWrite(MOTOR_CHIP_TWO_PWMA_B, 0);
                while (millis() - time < CARBON_DIOXIDE_IN_TIME_BEFORE_PERFUSION) {};
                digitalWrite(CARBON_DIOXIDE_RELAY, HIGH); // HIGH means closed-circuit
                setvalvedirection(MOTOR_CHIP_TWO_INPUT_B_ONE, MOTOR_CHIP_TWO_INPUT_B_TWO, false);
                analogWrite(MOTOR_CHIP_TWO_PWMA_B, 255); // CO2 close
                delay(5000);
                digitalWrite(CARBON_DIOXIDE_RELAY, LOW);
                analogWrite(MOTOR_CHIP_TWO_PWMA_B, 0);
                time = millis();
                digitalWrite(NITROGEN_RELAY, HIGH); // HIGH means closed-circuit
                setvalvedirection(MOTOR_CHIP_THREE_INPUT_A_ONE, MOTOR_CHIP_THREE_INPUT_A_TWO, true);
                analogWrite(MOTOR_CHIP_THREE_PWMA_A, 255); // N2 open
                delay(5000);
                digitalWrite(NITROGEN_RELAY, LOW);
                while (millis() - time < NITROGEN_IN_TIME_BEFORE_PERFUSION) {};
                setvalvedirection(MOTOR_CHIP_THREE_INPUT_A_ONE, MOTOR_CHIP_THREE_INPUT_A_TWO, false);
                analogWrite(MOTOR_CHIP_THREE_PWMA_A, 255); // N2 close
                delay(5000);
                digitalWrite(NITROGEN_RELAY, LOW);
                analogWrite(MOTOR_CHIP_THREE_PWMA_A, 0);
        }
        while (currentday <= CHECKPOINT_INTERVAL_TOTAL_SIZE) {
                time = millis();
                while (millis() - time < 86400000) {}
                time = millis();
                checkpoint_interval_index += 1;
                currentday = getCheckpoint(checkpoint_interval_index);
                analogWrite(MOTOR_CHIP_TWO_PWMA_A, 255); // Waste full
                while (millis() - time <= getMediaperfusion(perfusion_index)) {}
                analogWrite(MOTOR_CHIP_TWO_PWMA_A, 0); // Waste stop
                time = millis();
                analogWrite(MOTOR_CHIP_ONE_PWMA_B, 255); // Fresh media full
                while (millis() - time <= getMediaperfusion(perfusion_index)) {}
                analogWrite(MOTOR_CHIP_ONE_PWMA_B, 0); // Fresh media stop
                time = millis();
                digitalWrite(CARBON_DIOXIDE_RELAY, HIGH); // HIGH means closed-circuit
                setvalvedirection(MOTOR_CHIP_TWO_INPUT_B_ONE, MOTOR_CHIP_TWO_INPUT_B_TWO, true);
                analogWrite(MOTOR_CHIP_TWO_PWMA_B, 255); // CO2 open
                delay(5000);
                digitalWrite(CARBON_DIOXIDE_RELAY, LOW);
                analogWrite(MOTOR_CHIP_TWO_PWMA_B, 0);
                while (millis() - time < getMediaperfusion(perfusion_index) * AIR_OUT_PERCENTAGE_PERFUSION \
                * CARBON_DIOXIDE_IN_TIME_UPON_CHAMBER_TWO) {};
                digitalWrite(CARBON_DIOXIDE_RELAY, HIGH); // HIGH means closed-circuit
                setvalvedirection(MOTOR_CHIP_TWO_INPUT_B_ONE, MOTOR_CHIP_TWO_INPUT_B_TWO, false);
                analogWrite(MOTOR_CHIP_TWO_PWMA_B, 255); // CO2 close
                delay(5000);
                digitalWrite(CARBON_DIOXIDE_RELAY, LOW);
                analogWrite(MOTOR_CHIP_TWO_PWMA_B, 0);
                time = millis();
                digitalWrite(NITROGEN_RELAY, HIGH); // HIGH means closed-circuit
                setvalvedirection(MOTOR_CHIP_THREE_INPUT_A_ONE, MOTOR_CHIP_THREE_INPUT_A_TWO, true);
                analogWrite(MOTOR_CHIP_THREE_PWMA_A, 255); // N2 open
                delay(5000);
                digitalWrite(NITROGEN_RELAY, LOW);
                while (millis() - time < getMediaperfusion(perfusion_index) * AIR_OUT_PERCENTAGE_PERFUSION \
                * NITROGEN_IN_TIME_BEFORE_PERFUSION) {};
                setvalvedirection(MOTOR_CHIP_THREE_INPUT_A_ONE, MOTOR_CHIP_THREE_INPUT_A_TWO, false);
                analogWrite(MOTOR_CHIP_THREE_PWMA_A, 255); // N2 close
                delay(5000);
                digitalWrite(NITROGEN_RELAY, LOW);
                analogWrite(MOTOR_CHIP_THREE_PWMA_A, 0);
                perfusion_index += 1;
        }
}

void loop() {
    
}

int getCheckpoint(int index) {
        int array[CHECKPOINT_INTERVAL_TOTAL_SIZE] = CHECKPOINT_INTERVAL;
        return array[index];
}

float getMediaperfusion(int index) {
        float array[PERFUSION_FACTOR_SIZE] = PERFUSION_FACTOR;
        return array[index] * TOTAL_MEDIA_OUT_TIME_PERFUSION;
}

float getAirInperfusion(int index) {
        return getMediaperfusion(index) * AIR_OUT_PERCENTAGE_PERFUSION;
}

void setMotorchipdirection(int input_1, int input_2) {
        digitalWrite(input_1, HIGH);
        digitalWrite(input_2, LOW);
}

void setvalvedirection(int input_1, int input_2, boolean direction) {
        if (direction) {
                digitalWrite(input_1, HIGH);
                digitalWrite(input_2, LOW); // valve right
        } else {
                digitalWrite(input_1, LOW);
                digitalWrite(input_2, HIGH); // valve left
        }
}