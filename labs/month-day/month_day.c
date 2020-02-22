#include <stdio.h>
#include <stdlib.h>

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday){
        
        char *months[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

        printf("%s %d, %d \n",months[*pmonth],*pday,year);
        
}

int main(int argc, char **argv) {

		int year = atoi(argv[1]);
        int yearday = atoi(argv[2]);
        int *pday;
        int *pmonth;

        int auxSum = 0;

        int days_per_month[]={31,28,31,30,31,30,31,31,30,31,30,31};
        if (year%4==0){
                days_per_month[1]=29;
        }

        for(int i = 0;i<12;i++){
                 auxSum+=days_per_month[i];
                if(yearday<=auxSum){
                        auxSum-=days_per_month[i];
                        yearday-=auxSum;
                        pday = &yearday;
                        pmonth = &i;
                        break;
                }
        }

        month_day(year, yearday,pmonth,pday);
        

    return 0;
}
