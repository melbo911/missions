# make_mission

make_mission is a little perl script which can be used to create scenery files for the great "Heli Rescue Mission" (HRM) X-Plane plugin made by Kristian80.

First create a new scene using the X-Plane WorldEditor (WED).

Export the scenepack and then convert the dsf file to a text version.

	DSFTool --dsf2text newscebe.dsf dsf.txt

Put the dfs.txt together with the messages.txt into a separate folder.

Run the script:

	./make_mission newfolder > scenery_3.xml

Put the new XML file to the HRM plugin folder.

Done.







