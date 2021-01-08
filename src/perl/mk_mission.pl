#!/usr/bin/perl

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

# missions start with 0
$mission_number = 0;

# text messages
$title = "";
$type = "";
$start = "";
$pickup = "";
$flight2 = "";
$failed1 = "";
$failed2 = "";
$end = "";

# some global variables
@objects;
@o_num;
@o_lat;
@o_lon;
@o_head;
$min_lat = 9999999999;
$min_lon = 9999999999;
$max_lat = 0;
$max_lon = 0;
$lat_center = 0;
$lon_dcenter = 0;
$obcnt = 0;

#--------------------------------------

# print XML header
print '<?xml version="1.0" encoding="utf-8"?>
<scenery>
   <name>Custom</name>
</scenery>
';

#--------------------------------------

foreach $mission ( @ARGV ) {
   #print "doing $mission\n";
   do_mission($mission);
} 

#--------------------------------------

sub do_mission() {
   my ($m) = @_;
   if ( -f "$m/messages.txt" && -f "$m/dsf.txt" ) {
      print "<mission_$mission_number>\n";

      print_mission($m);
      read_objects($m);
      print_objects($m);

      print "</mission_$mission_number>\n";
      $mission_number++;
   }
}

#--------------------------------------

sub read_objects() {
   my ($m) = @_;

   my $obj = 0;

   $obcnt = 0;
   undef @objects,@o_num,@o_lat,@_lon,@o_head;

   open(DSF,"<$m/dsf.txt") or die;
   while(<DSF>) {
      chomp();
      if ( m/^OBJECT_DEF / ) {
         my($x,$opath) = split(' ');
         $objects[$obj++] = $opath;       
      } elsif ( m/^OBJECT / ) {
       
         my($x,$o,$x,$y,$z) = split(' ');

#print "o=$o x=$x y=$y z=$z\n";
         $x =~ s/^(.*)\.(.....).*/\1\2/;	# reduce size of latitude
         $y =~ s/^(.*)\.(.....).*/\1\2/;	# reduce size of longitute
         $z =~ s/\..*//g;
#print "o=$o x=$x y=$y z=$z\n";
         $o_num[$obcnt] = $o; 
         $o_lat[$obcnt] = $x; 
         $min_lat = $x if ($x < $min_lat);	# find max and min positions
         $max_lat = $x if ($x > $max_lat);
         $o_lon[$obcnt] = $y; 
         $min_lon = $y if ($y < $min_lon);
         $max_lon = $y if ($y > $max_lon);
         $o_head[$obcnt] = $z; 
         $obcnt++;
      }
   }
   close(DSF);

   # try to find center of scenery
   my $lat_diff = $max_lat - $min_lat;	
   my $lon_diff = $max_lon - $min_lon;
   $lat_center = int($lat_diff/2);
   $lon_center = int($lon_diff/2);
#  print " LAT $min_lat ($lat_diff)     LON : $min_lon ($lon_diff)\n";
}

#--------------------------------------

sub print_objects() {
   my $o = 0;
   while ( $o < $obcnt) {
      print "    <object_$o>
        <obj_path>$objects[$o_num[$o]]</obj_path>
        <elevation>0</elevation>
        <dist_x>".int($o_lat[$o] - $min_lat - $lat_center)."</dist_x>
        <dist_y>".int($o_lon[$o] - $min_lon - $lon_center)."</dist_y>
        <heading>$o_head[$o]</heading>
        <pitch>0</pitch>
        <roll>0</roll>
        <is_patient>true</is_patient>
        <is_slingload>false</is_slingload>
        <sling_is_bambi_bucket>false</sling_is_bambi_bucket>
        <sling_instanced_drawing>true</sling_instanced_drawing>
        <sling_pos_x>0</sling_pos_x>
        <sling_pos_y>0</sling_pos_y>
        <sling_pos_z>0</sling_pos_z>
        <sling_weight>100</sling_weight>
        <sling_height>0</sling_height>
        <sling_size_x>0</sling_size_x>
        <sling_size_y>0</sling_size_y>
        <sling_size_z>0</sling_size_z>
        <sling_cw_x>0</sling_cw_x>
        <sling_cw_y>0</sling_cw_y>
        <sling_cw_z>0</sling_cw_z>
        <sling_friction_glide>0.35</sling_friction_glide>
        <sling_friction_static>3</sling_friction_static>
    </object_$o>\n";
     $o++;
  }
}

#--------------------------------------

sub print_mission() {
   my ($m) = @_;
   require "$m/messages.txt";
   print "    <name>$title</name>
    <type>$type</type>
    <start_text>$start</start_text>
    <pickup_text>$pickup</pickup_text>
    <flight2_text>$flight2</flight2_text>
    <failed_arr_text>$failed1</failed_arr_text>
    <failed_hosp_text>$failed2</failed_hosp_text>
    <end_text>$end</end_text>
    <sound_start>0</sound_start>
    <sound_arr>0</sound_arr>
    <sound_pickup>0</sound_pickup>
    <sound_end>0</sound_end>
    <sound_failed_arr>0</sound_failed_arr>
    <sound_failed_hops>0</sound_failed_hops>
";
}
#--------------------------------------
