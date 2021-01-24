$$$wdk$$$ - SCAFFOLDER

Congratulations! You have just created the scaffold for a new
element!

What exactly does this mean?

The files in this archive represent all you need to create a
new element.

Follow the instructions to integrate the new element into
your existing website!


1. Extract Files

If not done already, extract the files from the archive.
The Folder "www" represents the root directory of the website.


2. Check assumptions

Before going forward please check the following assumptions. You
may have to adjust some of the instructions if one or more of these
assumptions are not true for your existing website.

* The source directory is a subdirectory of the website's root
directory. If not, make sure that all files from the archive in
/www/_source/ are copied to the right place!
* The assemblies directory is a subdirectory of /_source/.


3. Copy Files

Copy all files from the "www" folder to the document root folder of
your website.


4. Add the assembly to the website's assembly line

If not included already, the $$$a$$$ assembly must
be added to your website's assembly line - this is needed to load
the $$$ElementId$$$ element.

Usually the website class implementation is in a file named like this:
/_source/website_yourwebsite.inc


Within your website class implementation, search for the function
OnInitAssemblyLine(). If it doesn't exist add it:

	function OnInitAssemblyLine(&$assemblyLine)
	{
		$assembly = new CAssembly($this,'$$$a$$$');
		$assemblyLine->AddAssembly($assembly);
	}

If the function exists, just add the following lines to the existing
function:

	$assembly = new CAssembly($this,'$$$a$$$');
	$assemblyLine->AddAssembly($assembly);


5. Review the element in the element gallery

Try to review the new element in the element gallery.

The URL to the element gallery looks like this:
https://www.yourwebsite.com/?content=devtools-elementgallery

If you can't access the element gallery, try to add the
following code to the function OnCheckCondition():

			if ($strCondition == 'devtools')
			{
				return true;	
			}

6. Implement your element

An element implementation involves the following files:
* The element implementation in element_$$$elementid$$$.inc.
* The element demo code which is used in the element gallery in
  element_$$$elementid$$$_demo.inc.
* Layout files in the assembly's "layout" folder.

Review these files to get more hints and ideas on how the
implementation of an element works.


Check out $$$wdk-docs-url$$$ for further assistance!

Good Luck!
