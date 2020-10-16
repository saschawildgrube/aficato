$$$wdk$$$ - SCAFFOLDER

Congratulations! You have just created the scaffold for a new website!

Follow the instructions to get your new website online!

1. Extract Files

If not done already, extract the files from the archive.
The Folder "www" represents your document root. It may be called
differently in your server environment.
In Plesk it is usually called "httpdocs".


2. Copy Files

Copy all files from the "www" folder to the document root folder of
your server environment.


3. Check .htaccess support

The WDK scaffolder makes an assumption on the configuration of your web server:
It assumes that .htaccess files can be used to restrict access to folders
within the document root folder.

Try to request the following URL:
http://$$$rooturl$$$/$$$sitedir$$$_source/wdk/wdk.txt

If you see a "403 Authorization Failed" error message everything is ok.
It means that the "_source" folder is blocked against outside requests and
that the ErrorDocument configuration setting has been applied.

If you see the Website Development Kit readme file, you must either:
Reconfigure your webserver to allow the use of .htaccess for the contained
directives
OR
add the directives contained in /.htaccess and /_source/.htaccess to your
vhost configuration.
OR
move the "_source" folder outside the document root folder of your webserver
and otherwise override http error pages

If you do the latter, you must change include pathes. Search for "_source" in
all files to catch all.


4. Install the web application

A dynamic (i.e. database backed) web application must be installed by running
the install script once. The install script will run the install command for
all configured web services and perform other initial tasks like creating users.

http://$$$rooturl$$$/$$$sitedir$$$install/

Please note: Once you start adding features, especially when adding new entities
or when changing the data model, you may have to re-install your web application
during development.
In this case, delete all database tables and run the install script again.

Later when the web application is in production you will have to modify the
database tables manually or install new web services selectively.

The install script can only help when starting from scratch - i.e. with
an empty database.

The configuration setting TABLENAMEPREFIX in the file _source/database.cfg may
also come in handy. When a table name prefix is configured, all table names
used in sql queries are prefixed so you can have multiple versions of all
tables in one logical database.


5. Setup cronjobs for the scheduler

When installing web services some of them add scheduled jobs which are managed
by the system/scheduler web service.
But you also need a trigger for these jobs from the underlying operating
system. In a linux environment you can use crontab to trigger jobs.

Configure this to be executed every minute:
wget http://$$$rooturl$$$/$$$sitedir$$$webservices/system/scheduler/crontab/ --spider --timeout=55 --tries=1 --no-check-certificate


6. Start developing your website

A good start to develop your website is:
- Add menu items in /_source/navigation/navigation_default_int.txt
- Add corresponding text items in /_source/res/res_default_en.txt
- Add corresponding content htm or txt files in /_source/content/en/
- Start to implement the layout of your site by changing and adding files in /_source/layout/
- Run through /_source/website_$$$websitename$$$.inc. Change general configuration
  settings and modify the implementation of callback functions.
  You will find some hints in source code comments.
  
You may also want to explore the debug mode of your new website:
http://$$$rooturl$$$/$$$sitedir$$$?trace=1


Check out $$$wdk-docs-url$$$ for further assistance!

Good Luck!