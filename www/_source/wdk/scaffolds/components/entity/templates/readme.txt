$$$wdk$$$ - SCAFFOLDER

Congratulations! You have just created the scaffold for a new
data entity!

What exactly does this mean?

The files in this archive represent all you need to support a 
new data entity in a dynamic WDK based website.

The core element is a web service that encapsulates all database
access operations for the new entity.
Also included is a module that displays a list of items and which
enables a user to add, modify and delete data items.

The existing WDK based website into which you want to add this code
must have web service support already built in. The instructions below
assume that this is the case. So if not you will have to prepare your
website before getting started.

Follow the instructions to integrate the new data entity into
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
* A valid database configuration is available.


3. Copy Files

Copy all files from the "www" folder to the document root folder of
your website.


4. Add the web service into webservices_directory.inc

If your website uses web services it must support a function named
GetWebservicesDirectory() that adds web service configurations to an
array.

Typically GetWebservicesDirectory() is implemented in the file
/_source/webservices_directory.inc.

Add the following lines to the function (you may have to adjust
the array variable name depending on your implementation):

	$arrayWebservices["$$$webservicename$$$"]["url"]				= $strWebservicesRootURL . "$$$webservicename$$$/";
	$arrayWebservices["$$$webservicename$$$"]["accesscode"]	= "$$$accesscode$$$";


5. Add the assembly to the website's assembly line

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


6. Add content page to navigation

The scaffold contains a content page named "$$$entityname$$$manager".
The content page loads the $$$modulegroup$$$/$$$moduleid$$$ module
and displays it.

You may want to add this content page to your website's navigation
tree.
Usually the navigation file is /www/_source/navigation/navigation_default_int.txt.


7. Implement your entity

You may want to install the web service to see if you did
everything to integrate the module and the web service first.
If so jump to step 8 first.

An entity implementation involves the following files:
* The entity definition in entity_$$$entityname$$$.inc.
* The web service implementation in webservice_$$$entityname$$$.inc.
* The web service index file: /www/webservices/$$$ws1$$$/$$$ws2$$$/index.php
* The web service test scripts. Check out /assemblies/$$$a$$$/quality/tests/webservice-$$$ws1$$$-$$$ws2$$$/test_basic.php to get started.
* The module implementation in module_$$$moduleid$$$.inc.
* And as part of the module implementation the text resources in
res_module-$$$moduleid$$$_en.txt.

Read these files to get more hints and ideas on how the
implementation of an entity works.
The entity definition contains information about the supported
attributes (in other words: the columns of the database table) and other
aspects of the entity. The entity definition configures the CEntityManagerModule
based module and the webservice at the same time.

Keep in mind that changing the entity definition or web service implementation
always implies that the associated test scripts need to be changed, too.
It is good practice to create test scripts that cover all functions of a
web service.

Check out the web service implementation to see which callback functions
can be used to adjust the web service behaviour. The same applies to
the module implementation.


8. Install the web service

Before the web service will work properly (and thus the module) the web service
must be installed using the "install" command. The install command will create
the database table based on the entity definition.

Once completed, go to your website, open the $$$entityname$$$manager content
and see if everything works properly.

If you make changes to the entity definition - especially to the attributes - you
will have to reinstall the web service (i.e. drop and create the database table).
There is no uninstall command, so you will have to drop the table manually using
a database admin interface before running the "install" command again.


Check out $$$wdk-docs-url$$$ for further assistance!

Good Luck!