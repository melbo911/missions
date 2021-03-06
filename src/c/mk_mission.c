/*
#
# type 0 = Street Accidents
# type 1 = Urban Accidents
# type 2 = Search and Rescue
# type 3 = Sling Line
# type 4 = Fire Fighter Crash
# type 5 = Construction Site Fire
# type 6 = Cabin Fire
# type 7 = Moutain Fire
#
# is_patient = set to true if object should disapear after pickup
#
# melbo @ x-plane.org
#
*/
#define VERSION "1.4"

#ifdef _WIN32
 #ifndef F_OK
 # define F_OK 0
 # define X_OK 1
 # define W_OK 2
 # define R_OK 4
 #endif
#else
 #include <libgen.h>
 #include <unistd.h>
 #include <dirent.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OBJ 256
#define MAX_TXT 256

char title[MAX_TXT];
char type[MAX_TXT];
char start[MAX_TXT];
char pickup[MAX_TXT];
char flight2[MAX_TXT];
char failed1[MAX_TXT];
char failed2[MAX_TXT];
char end[MAX_TXT];

char objects[MAX_OBJ][MAX_TXT];
int o_num[MAX_OBJ];
double o_lat[MAX_OBJ];
double o_lon[MAX_OBJ];
int o_head[MAX_OBJ];

double min_lat   = 0;
double min_lon   = 0;
double max_lat   = 0;
double max_lon   = 0;
int lat_center = 0;
int lon_center = 0;
int obcnt      = 0;
int is_patient = 0;
int mission    = 0;
FILE *output;

int debug = 1;

/*-----------------------------------------------------------------*/

char* getword(char *s,int n) {           /* return n-th word */
   int l = strlen(s);
   int i = 0;
   char *p = s ;

   while ( n > 0 ) {
      p = &s[i];
      while (i < l && s[i] > ' ') {
         i++;
      }
      s[i++] = '\0';
      n--;
   }

   return(p);
}

/*-----------------------------------------------------------------*/

int strip(char *s) {           /* remove trailing whitespace */
   int i = strlen(s);
   while (i > 0 && s[i] < ' ') {
      s[i--] = '\0';
   }
   return(0);
}

/*-----------------------------------------------------------------*/

int read_messages(char *f)        /* quick and dirty file read */
{
   FILE *fp;
   if ( (fp = fopen(f,"r")) ) {
      fgets(title, MAX_TXT, fp);
      fgets(type, MAX_TXT, fp);
      fgets(start, MAX_TXT, fp);
      fgets(pickup, MAX_TXT, fp);
      fgets(flight2, MAX_TXT, fp);
      fgets(failed1, MAX_TXT, fp);
      fgets(failed2, MAX_TXT, fp);
      fgets(end, MAX_TXT, fp);
      strip(title);
      strip(type);
      strip(start);
      strip(pickup);
      strip(flight2);
      strip(failed1);
      strip(failed2);
      strip(end);
      return(0);
   } else {
      printf("cannot open %s\n",f);
      return(9);
   }
}

/*-----------------------------------------------------------------*/

int print_mission(char *m) 	     /* print mission header */
{
   char mpath[MAX_TXT];
   sprintf(mpath,"%s/messages.txt",m);
   if (!read_messages(mpath)) {

   fprintf(output,"    <name>%s</name>\n\
    <type>%s</type>\n\
    <start_text>%s</start_text>\n\
    <pickup_text>%s</pickup_text>\n\
    <flight2_text>%s</flight2_text>\n\
    <failed_arr_text>%s</failed_arr_text>\n\
    <failed_hosp_text>%s</failed_hosp_text>\n\
    <end_text>%s</end_text>\n\
    <sound_start>0</sound_start>\n\
    <sound_arr>0</sound_arr>\n\
    <sound_pickup>0</sound_pickup>\n\
    <sound_end>0</sound_end>\n\
    <sound_failed_arr>0</sound_failed_arr>\n\
    <sound_failed_hops>0</sound_failed_hops>\n\
",title,type,start,pickup,flight2,failed1,failed2,end);

   } else {
     printf("oops... cannot read messages\n");
   }
   return(0);
}

/*-----------------------------------------------------------------*/

int read_objects(char *m)
{
   char mpath[MAX_TXT];
   char buf[MAX_TXT];
   char buf2[MAX_TXT];
   sprintf(mpath,"%s/dsf.txt",m);

   int obj = 0;
   obcnt = 0;
   float x = 0;
   float y = 0;
   int z = 0;

   min_lat = 9999999.0;
   min_lon = 9999999.0;
   max_lat = -9999999.0;
   max_lon = -9999999.0;
   lat_center = 0;
   lon_center = 0;
   int n_patient = 0;
   
   FILE *fp;
   if ( (fp = fopen(mpath,"r")) ) {
      while ( fgets(buf, MAX_TXT, fp) ) {
         strip(buf);
         if ( ! strncmp(buf,"OBJECT_DEF",10) ) {
            strcpy(objects[obj++],&buf[11]);
         } else {
            if ( ! strncmp(buf,"PROPERTY sim/require_object ",28) ) {
               if ( ! strncmp(&buf[28],"6/",2) ) {
                  is_patient = n_patient;
               } else {
                  n_patient = atoi(&buf[30]);
               }
            } else {
               if ( ! strncmp(buf,"OBJECT ",7) ) {
                  strcpy(buf2,buf);
                  o_num[obcnt] = atoi(getword(buf2,2));

                  strcpy(buf2,buf);
                  x = atof(getword(buf2,3)) * 100000;

                  strcpy(buf2,buf);
                  y = atof(getword(buf2,4)) * 100000;

                  strcpy(buf2,buf);
                  z = atoi(getword(buf2,5));

                  if ( x < min_lat ) min_lat = x;
                  if ( x > max_lat ) max_lat = x;

                  if ( y < min_lon ) min_lon = y;
                  if ( y > max_lon ) max_lon = y;


                  o_lat[obcnt] = x;

                  o_lon[obcnt] = y;

                  o_head[obcnt] = z;
   
                  obcnt++;
               }
            }
         }
      }
      lat_center = (int)((max_lat - min_lat) / 2 );
      lon_center = (int)((max_lon - min_lon) / 2 );

      if ( debug ) {
        printf("maxlon=%lf minlon=%lf lonctr=%d\n",max_lon,min_lon,lon_center);
        printf("maxlat=%lf minlat=%lf latctr=%d\n",max_lat,min_lat,lat_center);
      }
   }
   return(0);
}

/*-----------------------------------------------------------------*/

int print_objects(char *m)
{
   int o = 0;
   char patient[10];
   while ( o < obcnt ) {
      if ( is_patient > 0 ) {
        strcpy(patient,"true");
        is_patient--;
      } else {
        strcpy(patient,"false");
      }

      fprintf(output,"    <object_%d>\n\
        <obj_path>%s</obj_path>\n\
        <elevation>0</elevation>\n\
        <dist_x>%d</dist_x>\n\
        <dist_y>%d</dist_y>\n\
        <heading>%d</heading>\n\
        <pitch>0</pitch>\n\
        <roll>0</roll>\n\
        <is_patient>%s</is_patient>\n\
        <is_slingload>false</is_slingload>\n\
        <sling_is_bambi_bucket>false</sling_is_bambi_bucket>\n\
        <sling_instanced_drawing>true</sling_instanced_drawing>\n\
        <sling_pos_x>0</sling_pos_x>\n\
        <sling_pos_y>0</sling_pos_y>\n\
        <sling_pos_z>0</sling_pos_z>\n\
        <sling_weight>100</sling_weight>\n\
        <sling_height>0</sling_height>\n\
        <sling_size_x>0</sling_size_x>\n\
        <sling_size_y>0</sling_size_y>\n\
        <sling_size_z>0</sling_size_z>\n\
        <sling_cw_x>0</sling_cw_x>\n\
        <sling_cw_y>0</sling_cw_y>\n\
        <sling_cw_z>0</sling_cw_z>\n\
        <sling_friction_glide>0.35</sling_friction_glide>\n\
        <sling_friction_static>3</sling_friction_static>\n\
    </object_%d>\n",
         o,objects[o_num[o]],(int)(o_lat[o] - min_lat - lat_center),(int)(o_lon[o] - min_lon - lon_center), o_head[o], patient, o);

      o++;
   }

   return(0);
}

/*-----------------------------------------------------------------*/

int do_mission(char *m)
{
   char mpath[MAX_TXT];

   if (debug) printf("doing mission %s\n",m);

   sprintf(mpath,"%s/messages.txt",m);
   if( access( mpath, F_OK ) == -1 ) {
      printf("%s does not exist\n",mpath);
      return(11);
   }
   sprintf(mpath,"%s/dsf.txt",m);
   if( access( mpath, F_OK ) == -1 ) {
      printf("%s does not exist\n",mpath);
      return(12);
   }
   fprintf(output,"<mission_%d>\n",mission);

   print_mission(m);
   read_objects(m);
   print_objects(m);

   fprintf(output,"</mission_%d>\n",mission);
   mission++;
   return(0);
}

/*-----------------------------------------------------------------*/

int print_head()
{
   fprintf(output, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<scenery>\n   <name>Custom</name>\n</scenery>\n");
   return(0);
}

/*-----------------------------------------------------------------*/

int main(int argc, char **argv) {

   if ( argc < 2 ) {
      printf("usage: %s params....\n     Version: %s\n",argv[0],VERSION);
      return(1);
   }

   if ( (output = fopen("new-scenery.xml","w")) != NULL ) {

      print_head();

      int i=1;
      while (i<argc) {
         do_mission(argv[i]);
         i++;
      }
      fclose(output);
   } else {
      printf("cannot open output file\n");
   }
}


