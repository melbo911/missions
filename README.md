# m_mission

mk_mission is a little program (perl script and compiled version available)  which can be used to create scenery files for the great "Heli Rescue Mission" (HRM) X-Plane plugin made by Kristian80.


First create a new scene using the X-Plane WorldEditor (WED).

Export the scenepack and convert the dsf file inside to a text version. YOu will need "DSFtool" from the XPTools package (https://developer.x-plane.com/tools/xptools/).

	DSFTool --dsf2text newscene.dsf dsf.txt

Put the dfs.txt together with the messages.txt into a separate folder.

Run the script/program:

	mk_mission newfolder > scenery_3.xml

Put the created XML file to the HRM plugin folder. In addition to "Civilian" and "Military" you will now see a "Custom" mission set in the pulldown list. The "Title" can easily be changed inside the XML file using a text editor.

Done.

The source folders contain some examples. They're called 001, 002 and 003

Note: The message.txt file format for the  Perl script uses a different format. Just look at the example folders.




