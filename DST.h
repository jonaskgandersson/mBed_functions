
/* Returns true if date is in daylight saving time
   DST last sunday in March to last sunday in October
   
   hour: current hour(0-23)
   day: current day in mounth
   month: current month
   dow: current day of week( 0=Sunday )
*/
bool IsDst(int hour, int day, int month, int dow)
{
     if (month < 3 || month > 10)  return false; 
     if (month > 3 && month < 10)  return true; 

     int previousSunday = day - dow;

     if (month == 3)
     {
        if( (previousSunday >= 25) && (dow == 0) && (hour>=2) )
        {
           return FALSE;
        }
        else if(previousSunday >= 25)
        {
           return FALSE;
        }
        else
        {
           return TRUE;
        }
     }
           
     if (month == 10)
     {
        if( (previousSunday < 25) && (dow // To do

     return false; // this line never gonna happend
 }
