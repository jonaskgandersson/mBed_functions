
/* Returns true if date is in daylight saving time
   DST last sunday in March to last sunday in October
   
   day: current day in mounth
   month: current month
   dow: current day of week( 0=Sunday )
*/
bool IsDst(int day, int month, int dow)
{
     if (month < 3 || month > 10)  return false; 
     if (month > 3 && month < 10)  return true; 

     int previousSunday = day - dow;

     if (month == 3) return previousSunday >= 25;
     if (month == 10) return previousSunday < 25;

     return false; // this line never gonna happend
 }
