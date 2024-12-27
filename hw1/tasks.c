#include <stdio.h>

int main(void) {

    //intializing variables
    int MAX_TYPES = 10;
    int jar_size = 0;
    int jar_total = 0;
    int time_left = 0;
    int num_tasks = 0;
    char identifier = 0;
    int duration = 0;
    char identifiers[MAX_TYPES];
    int durations[MAX_TYPES];
    int num_tasks_jar[MAX_TYPES];
    char command = 0;
    int num_times = 0;
    int check = 0;
    int X, YY, H, MM;

    //gets the jar size and returns errors if invalid
    printf("How many minutes to spend? ");
    if ( (scanf(" %d", &jar_size) != 1) || (jar_size < 0) || (jar_size > 600)) {    
        fprintf(stderr,"Invalid input\n");
        return 1;
    }
    time_left = jar_size;

    //prompting the user for tasks and durations
    printf("Enter task type identifiers and durations in minutes,\n");
    printf("one per line; enter '0 0' to stop\n");

    //save tasks entered and break if conditions met
    while(num_tasks < MAX_TYPES){
        if( scanf(" %c%d", &identifier, &duration) != 2) {
            fprintf(stderr, "Invalid input\n");
            return 1;
        }
        if (((identifier == '0') || (identifier == EOF)) &&
            ((duration == 0) || (duration == EOF))) {
            break;
        }

        //store the values and do the next iteration
        identifiers[num_tasks] = identifier;
        durations[num_tasks] = duration;
        num_tasks_jar[num_tasks] = 0;
        num_tasks++;
    }

    //enters command loop and prompts user for command
    printf("Enter a command [a, r, s, q]: ");
    while(scanf(" %c", &command) != EOF)  {
        int array_val = 0;
        //switches to command that user inputed
        switch(command){
            //case that adds values to the jar 
            case 'a':
                //checks identifier and num times values to see if valid and returns error if not
                if(scanf(" %c%d", &identifier, &num_times) != 2 ||
                    (num_times < 0 )){
                    fprintf(stderr, "Invalid input\n");
                    return 1;
                }
                for(int i = 0; i < num_tasks; i++) {
                    if (identifiers[i] == identifier) {
                        array_val = i;
                        check = 1;
                        break;
                    }
                }

                //error handling
                if(check != 1){
                    fprintf(stderr, "Unknown task type identifier\n");
                    return 2;
                }

Instructor
| 09/21 at 11:02 am
Grading comment:
You should reset check to 0 for the next time you search for an identifier. Otherwise, check would still be 1 and the error would not get caught.

                //adding value to the jar if it fits and outputting error if not
                if(num_times * durations[array_val] < time_left){                   
Instructor
| 09/21 at 11:08 am
Grading comment:
This comparison should use <= since you can use the whole jar but not more.

                    num_tasks_jar[array_val] += num_times;
                    time_left -= num_times * durations[array_val];
                    jar_total += num_times * durations[array_val];
                } else {
                    fprintf(stderr, "Invalid command\n");
                    break;
                }
                break;
            //case that removes values from the jar
            case 'r':
                //checks identifier and num times values to see if valid and returns error if not
                if(scanf(" %c%d", &identifier, &num_times) != 2 ||
                    (num_times < 0 )){
                    fprintf(stderr, "Invalid input\n");
                    return 1;
                }
                for(int i = 0; i < num_tasks; i++) {
                    if (identifiers[i] == identifier) {
                        array_val = i;
                        check = 1;
                        break;
                    }
                }

                //error handling
                if(check != 1){
                    fprintf(stderr, "Unknown task type identifier\n");
                    return 2;
                }

Instructor
| 09/21 at 10:49 am
Grading comment:
Did not reset check to 0.

                //removing value from the jar if it can and outputting error if not
                if(num_tasks_jar[array_val] - num_times >= 0){                   
                    num_tasks_jar[array_val] -= num_times;
                    time_left += num_times * durations[array_val];
                    jar_total -= num_times * durations[array_val];
                } else {
                    fprintf(stderr, "Invalid command\n");
                    break;
                }
                break;

            //s case which reads out the info for each
            case 's':
                printf("Identifier,Duration,Count,Total Time\n");
                for(int i = 0; i < num_tasks; i++) {
                    printf("%c,%d,%d,%d\n", identifiers[i], durations[i],
                        num_tasks_jar[i], durations[i] * num_tasks_jar[i] );
                }

                //calculates and prints out time remaining
                X = jar_total / 60;
                YY = jar_total % 60;
                H = jar_size / 60;
                MM = jar_size % 60;
                printf("Total time in jar: %d:%02d out of: %d:%02d possible\n",
                    X, YY, H, MM);
                break;
            //quit case that exits and returns 0
            case 'q':
                printf("Bye!\n");
                return 0;
            //case for invalid input
            default:
                fprintf(stderr, "Invalid command\n");
                break;
        }
        //print command prompt for next loop
        printf("Enter a command [a, r, s, q]: ");
    }

//print bye and return 0 at end of code for ctrl-d press
printf("Bye!\n");
return 0;
}
