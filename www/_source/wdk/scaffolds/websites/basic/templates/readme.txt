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
Reconfigure your webserver to allow .htaccess based access restrictions
and error documents
OR
move the "_source" folder outside the document root folder of your webserver.

If you do the latter, you must change include pathes. Search for "_source" in
all files to catch all.


4. Start developing your website

A good start to develop your website is:
- Add menu items in /_source/navigation/navigation_default_int.txt
- Add corresponding text items in /_source/res/res_default_en.txt
- Add corresponding content htm or txt files in /_source/content/en/
- Start to implement the layout of your site by changing and adding files in /_source/layout/
- Run through /_source/website_$$$websitename$$$.inc

There You can change config settings and work on the implementation of
overloaded functions. You will find some hints in source code comments.


Check out $$$wdk-docs-url$$$ for further assistance!

Good Luck!