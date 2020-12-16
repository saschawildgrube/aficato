$$$wdk$$$ - SCAFFOLDER

Congratulations! You have just created the scaffold for a new
module!

What exactly does this mean?

The files in this archive represent all you need to create a
new module with different states.

Follow the instructions to integrate the new module into
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
the $$$ModuleID$$$ module.

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


5. Add content page to navigation

The scaffold contains a content page named "$$$moduleid$$$".
The content page loads the $$$modulegroup$$$/$$$moduleid$$$ module
and displays it.

You may want to add this content page to your website's navigation
tree.
Usually the navigation file is /www/_source/navigation/navigation_default_int.txt.


6. Implement your module

A module implementation involves the following files:
* The module implementation in module_$$$moduleid$$$.inc.
* The text ressources in res_module-$$$moduleid$$$_en.txt.
* Layout files in the assembly's "layout" folder.

Read these files to get more hints and ideas on how the
implementation of a module works.


Check out $$$wdk-docs-url$$$ for further assistance!

Good Luck!
