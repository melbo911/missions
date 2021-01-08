# mk_mission

mk_mission is a little program, which can be used to create scenery files for the great "Heli Rescue Mission" (HRM) X-Plane plugin made by Kristian80.


First create a new scene using the X-Plane WorldEditor (WED).

Export the scenepack and convert the dsf file inside to a text version. You will need "DSFtool" from the XPTools package (https://developer.x-plane.com/tools/xptools/).

	DSFTool --dsf2text newscene.dsf dsf.txt

Put the dfs.txt together with the messages.txt into a separate folder.

Run the program and give the created folder names as arguments, while redirecting the output to a file:

	mk_mission newfolder > scenery_3.xml

Put the created XML file to the HRM plugin folder. In addition to "Civilian" and "Military" you will now see a "Custom" mission set in the pulldown list. The title "Custom" can easily be changed inside the XML file using a text editor.

Done.


--------------------------------------------------------------

The C source folder contains some examples. They're called 001, 002 and 003

Folder 000 contains a message.txt file explaining the structure. The file is read top-to-down and messages are interpreted by their position inside the file.



Explanation:

	This line defines the mission title.
	This line defines the mission-type , i.e. "0" which stands for "Street Accidents" (see source)
	This line defines the short description of the mission.
	This line defines the message shown if we reach the scene on time.
	This line defines the message shown if the patient has been picked up.
	This line defines the message shown if we did not reach the scene on time.
	This line defines the message shown if we did not reach the hospital on time.
	This line defines the message shown if we reached the hospital on time.



Example:

	Frontal Crash
	0
	Frontal car crash in the country site. The fire fighters are already on site and reported one person to be seriously injured.
	Great, we made in time. Lets get the patient on board.
	The patient is on board and we are on our way to the hospital. We should go as quickly, but also as smoothly as possible to ensure our patient is alright.
	We did not make it in time. At least we should pick up the corpse, the hospital pathologist is eager to see it and we still get some money from the insurance.
	Unfortunately, our patient did not make it. Lets drop the corpse at the hospital and leave the rest to the pathologist.
	The patient was handed over to the hospital surgeons.
	
--------------------------------------------------------------

Special "patient" handling.

If the patient has been picked up, it will disapear from the scene. In order to define which object should be treated "as patient" it can be defined within WED. Since there is no "is_patient" property in WED, I'm using a different method. In X-Plane an object can be configured to be "shown with" a certain level of object density. These levels reach from 0 (Default) to 6 (Totally insane). I'm using level-6 to identify which object should be treated as "patient". For these objects the variable "is_patient" is being set to "true" in the generated XML file.






