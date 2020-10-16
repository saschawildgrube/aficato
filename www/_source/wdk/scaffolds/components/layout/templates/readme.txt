$$$wdk$$$ - SCAFFOLDER

Congratulations! You have just created the scaffold for a new
layout!


What exactly does this mean?

The files in this archive represent all you need to create a
new layout.

Follow the instructions to integrate the new layout into
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
the $$$LayoutID$$$ layout.

Usually the website class implementation is in a file named like this:
/_source/website_yourwebsite.inc


Within your website class implementation, search for the function
CallbackInitAssemblyLine. If it doesn't exist add it:

	function CallbackInitAssemblyLine(&$assemblyLine)
	{
		$assembly = new CAssembly($this,"$$$a$$$");
		$assemblyLine->AddAssembly($assembly);
	}

If the function exists, just add the following lines to the existing
function:

	$assembly = new CAssembly($this,"$$$a$$$");
	$assemblyLine->AddAssembly($assembly);


5. Initialize your new layout

Go to your website's implementation file and add the
layout to the list of supported layouts by adding this line
in the __construct() function:

$arrayConfig["layouts"][] = "$$$layoutid$$$";

You may even want to set the layout as the default layout by
adding this line in the CallBackOnInit() function:

$this->SetLayout("$$$layoutid$$$");


6. Implement your layout

A layout implementation involves the following files:

* The layout specific code in layout_$$$layoutid$$$.inc.
* The layout css file: layout-$$$layoutid$$$.css.
* The layout configuration file layout-$$$layoutid$$$.cfg.
* Any additional layout file that overrides default layout files

Look at these files to get an idea on how the implementation of a
layout works.


Check out $$$wdk-docs-url$$$ for further assistance!

Good Luck!
