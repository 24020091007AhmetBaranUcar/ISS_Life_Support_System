#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Oxygen thresholds
#define O2_TARGET 21.0       // percentage of air mass -- EARTH STANDARD
#define O2_MIN_SAFE 19.0     // Yellow alert -- NASA Hypoxia warning limit
#define O2_MIN_CRITICAL 17.0 // Red alert! --  Risk of loss of consciousness - Emergency
#define O2_MAX_SAFE 26.0
#define O2_MAX_CRITICAL 30.0
#define OGS_GENERATION_RATE 0.25

#define TOTAL_AIR_MASS 1000.0  //total kg of air in iss. this will be used for gas percentage calculations.

// CO2 thresholds
#define CO2_TARGET 0.04     //Percentage -- Earth Standard
#define CO2_MAX_SAFE 0.4     //  NASA OCHMO headache limit
#define CO2_MAX_CRITICAL 1.0 // Risk of poisoning -- emergency
#define CDRA_REMOVAL_RATE 0.30  // kg/h (Hourly CO2 cleaning capacity of scrubber)

// Pressure thresholds
#define PRESSURE_MAX_SAFE 103.0  // kPa
#define PRESSURE_MIN_SAFE 26.2

// Temperature thresholds
#define TEMP_MIN_SAFE 18.0
#define TEMP_TARGET 22.0       // °C
#define TEMP_MAX_SAFE 27.0
#define TEMP_MAX_CRITICAL 28.0

// Humidity thresholds
#define HUMIDITY_MIN 25.0      // %
#define HUMIDITY_MAX 70.0


typedef enum {
    SYSTEM_NORMAL,
    SYSTEM_WARNING,
    SYSTEM_CRITICAL,
    SYSTEM_EMERGENCY
}System_Threat_Level;

typedef union {
    uint8_t all;
    struct {
        uint8_t low_O2              :1;
        uint8_t high_O2             :1;
        uint8_t high_CO2            :1;
        uint8_t pressure_level_dangerous      :1;
        uint8_t temperature_level_dangerous   :1;
        uint8_t humidity_level_dangerous     :1;
        uint8_t dangerous_remaining_water_level :1;
        uint8_t reserved :1;
    }system_state;
}AlarmStatus;

typedef struct {
    float O2_level; //% percent
    float CO2_level; //% percent
    float pressure; //kPa
    float temperature; //Celcius
    float humidity; //% percent
    float water_reserve; // liters
    AlarmStatus alarm_status;
    System_Threat_Level system_threat_level;
}LifeSupport;

typedef enum {
    ACTIVITY_SLEEPING,
    ACTIVITY_RESEARCHING,
    ACTIVITY_EXERCISE
}ActivityStatus;

typedef enum {
    STATUS_HEALTHY,
    STATUS_SICK,
    STATUS_CRITICAL
}MemberSanitaryStatus;

typedef struct {
    char name[50];
    float O2_consume_speed; // kg/h
    float CO2_produce_speed; //kg/h
    ActivityStatus activity_status;
    MemberSanitaryStatus member_sanitary_status;
}CrewMember;

typedef struct {
    char name[125];
    uint8_t isActive; // 0 or 1
    float efficiency; // %
    float energy_consumption; //kW
    uint8_t isDefective; // 0 or 1
}SystemComponents;

//=====================================
//==============FUNCTIONS==============
//=====================================

void init_life_support(LifeSupport *ls) {
    ls->system_threat_level = SYSTEM_NORMAL; // system state is considered normal by default
    ls->O2_level = 20.0; // a random value smaller than targeted o2 level
    ls->CO2_level = 0.2; //a random value smaller than max safe co2 level
    ls->water_reserve = 1000.0;
    ls->humidity = 60.0;
    ls->pressure = 80.0; //a random vallue between min safe and max safe value of pressure
    ls->temperature = TEMP_TARGET;
}

void init_crew(CrewMember *crew, const char *name, float o2_consume_rate, float co2_produce_rate) {
    strncpy(crew->name, name, sizeof(crew->name) - 1);
    crew->name[sizeof(crew->name) - 1] = '\0'; // prevents buffer overflow
    crew->O2_consume_speed = o2_consume_rate;
    crew->CO2_produce_speed = co2_produce_rate;
    crew->activity_status = ACTIVITY_RESEARCHING;
    crew->member_sanitary_status = STATUS_HEALTHY;
}

void init_system_component(SystemComponents *sc, const char *name, uint8_t active, uint8_t defective, float efficiency, float power_consumption) {
    strncpy(sc->name, name, sizeof(sc->name) - 1);
    sc->name[sizeof(sc->name) - 1] = '\0';
    sc->isActive = active;
    sc->isDefective = defective;
    sc->efficiency = efficiency;
    sc->energy_consumption = power_consumption;
}

void update_oxygen(LifeSupport *ls, uint8_t crew_count, CrewMember crew[], SystemComponents *o2_generator, float dt) {
    //dt measures time passed based on hour
    float oxygen_consumed = 0.0;
    float oxygen_produced = 0.0;

    for (uint8_t i = 0; i < crew_count; i++) {
        float multiplier = 1.0;

        if (crew[i].activity_status == ACTIVITY_SLEEPING) {
            multiplier = 0.5;
        }
        else if (crew[i].activity_status == ACTIVITY_RESEARCHING) {
            multiplier = 1.0;
        }
        else if (crew[i].activity_status == ACTIVITY_EXERCISE) {
            multiplier = 2.0;
        }
        oxygen_consumed += multiplier * crew[i].O2_consume_speed * dt;
    }

    if (o2_generator->isActive && !o2_generator->isDefective) {
        oxygen_produced += OGS_GENERATION_RATE * (o2_generator->efficiency / 100.0) * dt;
    }

     ls->O2_level += ((oxygen_produced - oxygen_consumed) * 100 ) / TOTAL_AIR_MASS;
}

void update_CO2(LifeSupport *ls, uint8_t crew_count, CrewMember crew[], SystemComponents *co2_scrubber, float dt) {
    float co2_produced_kg = 0.0;
    float co2_removed_kg = 0.0;

    for (uint8_t i = 0; i < crew_count; i++) {
        float multiplier = 1.0;

        if (crew[i].activity_status == ACTIVITY_RESEARCHING) {
            multiplier = 1;
        }
        else if (crew[i].activity_status == ACTIVITY_EXERCISE) {
            multiplier = 2.0;
        }
        else if (crew[i].activity_status == ACTIVITY_SLEEPING) {
            multiplier = 0.5;
        }
        co2_produced_kg += multiplier * crew[i].CO2_produce_speed * dt;
    }

    //Is scrubber active and ready to work?
    if (co2_scrubber->isActive && !co2_scrubber->isDefective) {
        //amount of cleaned co2
        co2_removed_kg += CDRA_REMOVAL_RATE * (co2_scrubber->efficiency / 100.0f) * dt;
    }
    //update percentage
    float net_change_kg = co2_produced_kg - co2_removed_kg;
    ls->CO2_level += (net_change_kg * 100.0f) / TOTAL_AIR_MASS;
    //level of co2 cant be negative
    if (ls->CO2_level < 0.0f) {
        ls->CO2_level = 0.0f;
    }
}

void check_alarms(LifeSupport *ls) {
    ls->alarm_status.all = 0;
    if (ls->O2_level < O2_MIN_CRITICAL) {
        ls->alarm_status.system_state.low_O2 = 1;
    } if (ls->O2_level > O2_MAX_CRITICAL) {
        ls->alarm_status.system_state.high_O2 = 1;
    } if (ls->CO2_level > CO2_MAX_SAFE) {
        ls->alarm_status.system_state.high_CO2 = 1;
    } if (ls->humidity > HUMIDITY_MAX || ls->humidity < HUMIDITY_MIN) {
        ls->alarm_status.system_state.humidity_level_dangerous = 1;
    } if (ls->pressure > PRESSURE_MAX_SAFE || ls->pressure < PRESSURE_MIN_SAFE) {
        ls->alarm_status.system_state.pressure_level_dangerous = 1;
    } if (ls->temperature < TEMP_MIN_SAFE || ls->temperature > TEMP_MAX_CRITICAL) {
        ls->alarm_status.system_state.temperature_level_dangerous = 1;
    } if (ls->water_reserve < 15.0) { //if water reserve is about to end
        ls->alarm_status.system_state.dangerous_remaining_water_level = 1;
    }

    if (ls->alarm_status.system_state.low_O2 || ls->alarm_status.system_state.high_O2 || ls->alarm_status.system_state.high_CO2 ||
        ls->alarm_status.system_state.pressure_level_dangerous) {
        ls->system_threat_level = SYSTEM_EMERGENCY;
    } else if (ls->alarm_status.system_state.temperature_level_dangerous || ls->alarm_status.system_state.dangerous_remaining_water_level) {
        ls->system_threat_level = SYSTEM_CRITICAL;
    } else if (ls->alarm_status.system_state.humidity_level_dangerous) {
        ls->system_threat_level = SYSTEM_WARNING;
    } else {
        ls->system_threat_level = SYSTEM_NORMAL;
    }
}
//=====================================
//=============MAIN BLOCK=============
//=====================================
int main(void) {
    // --- 1. SYSTEM SETUP ---
    LifeSupport iss;
    init_life_support(&iss);

    CrewMember crew[2];
    init_crew(&crew[0], "Cmdr. Yilmaz", 0.045, 0.050);
    init_crew(&crew[1], "Eng. Baran", 0.040, 0.045);

    // NASA Hardware Setup
    SystemComponents OGS;
    init_system_component(&OGS, "NASA OGS Module", 1, 0, 100.0, 0.5);

    SystemComponents CDRA;
    init_system_component(&CDRA, "NASA CDRA Module", 1, 0, 100.0, 0.3);

    // ==========================================
    //  INITIALIZE LOG FILE
    // ==========================================
    FILE *log_file = fopen("telemetry_log.txt", "w");
    if (log_file == NULL) {
        printf("CRITICAL ERROR: Log file could not be created!\n");
        return 1; // Safely exit if file cannot be opened
    }

    // Print log file header
    fprintf(log_file, "========== NASA ECLSS TELEMETRY LOG ==========\n");
    fprintf(log_file, "TIME |   O2 %%   |  CO2 %%  | THREAT | ALARM BYTE\n");
    fprintf(log_file, "----------------------------------------------\n");

    printf("========== NASA ECLSS SIMULATION ACTIVE ==========\n");
    printf("Initial: O2=%.2f%% | CO2=%.3f%%\n\n", iss.O2_level, iss.CO2_level);

    // --- 2. TIME LOOP ---
    float dt = 1.0;

    for (int hour = 1; hour <= 30; hour++) {
        printf("--- MISSION TIME: T+%d ---\n", hour);

        // -- Scenario Events --
        if (hour == 3) {
            printf("[INFO] Eng. Baran started heavy exercise.\n");
            crew[1].activity_status = ACTIVITY_EXERCISE;
            fprintf(log_file, ">>> EVENT: Heavy exercise started\n"); // Log event
        }
        if (hour == 6) {
            printf(">>> [CRITICAL FAILURE] CDRA (CO2 Scrubber) Malfunction! <<<\n");
            CDRA.isDefective = 1;
            fprintf(log_file, ">>> EVENT: CDRA FAILURE!\n"); // Log failure
        }

        // -- Physics Updates --
        update_oxygen(&iss, 2, crew, &OGS, dt);
        update_CO2(&iss, 2, crew, &CDRA, dt);

        // -- Alarm Watchdog --
        check_alarms(&iss);

        // ==========================================
        //WRITING TO LOG
        // ==========================================

        fprintf(log_file, "T+%02d | %06.3f%% | %06.3f%% |   %02d   |   0x%02X\n",
                hour, iss.O2_level, iss.CO2_level, iss.system_threat_level, iss.alarm_status.all);

        // -- Console Output --
        printf("O2 Level:  %.3f%%\n", iss.O2_level);
        printf("CO2 Level: %.3f%%\n", iss.CO2_level);

        if (iss.system_threat_level == SYSTEM_EMERGENCY) {
            printf("!!! SYSTEM STATUS: EMERGENCY (Byte: 0x%02X) !!!\n", iss.alarm_status.all);
            if (iss.alarm_status.system_state.low_O2) printf(" -> Emergency: Hypoxia Risk!\n");
            if (iss.alarm_status.system_state.high_CO2) printf(" -> Emergency: CO2 Poisoning Risk!\n");
        }
        else if (iss.system_threat_level == SYSTEM_CRITICAL) {
            printf("!! SYSTEM STATUS: CRITICAL (Byte: 0x%02X) !!\n", iss.alarm_status.all);
        }
        else if (iss.system_threat_level == SYSTEM_WARNING) {
            printf("! SYSTEM STATUS: WARNING (Byte: 0x%02X) !\n", iss.alarm_status.all);
        }
        else {
            printf("System Status: NORMAL\n");
        }
        printf("\n");
    }

    fprintf(log_file, "\n========== END OF SIMULATION ==========\n");
    fclose(log_file);

    printf("========== SIMULATION COMPLETE ==========\n");
    printf("[INFO] Telemetry data has been saved to 'telemetry_log.txt'\n");

    return 0;
}