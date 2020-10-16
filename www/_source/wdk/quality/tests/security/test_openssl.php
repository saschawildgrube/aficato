<?php
	
	require_once(GetWDKDir().'wdk_unittest.inc');
	require_once(GetWDKDir().'wdk_openssl.inc');
		
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('ssl vulnerabilities');
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();
		
			$this->Trace('');
			$this->Trace("This test requires that the www user (e.g. www-data) can execute the\nopenssl command with root privileges.");
			$this->Trace("In order to do this, you'll need to create a file\nin /etc/sudoers.d/ with mode 0440 that contains the following line:");
			$this->Trace("www-data ALL=(ALL) NOPASSWD: /usr/bin/openssl");
			$this->Trace('');

		
			$arrayVersion = GetVersionInfoOpenSSL();
			$strBuildDate = ArrayGetValue($arrayVersion,'BUILD_DATE');
			$this->Trace('OpenSSL build date: '.$strBuildDate);
			if ($strBuildDate == '')
			{
				$this->Trace('The build date could not be determined.');
				$this->SetResult(false);
				return;	
			}
			if ($strBuildDate < '2014-04-07')
			{
				$this->Trace("OpenSSL build date is before April 7th 2014 and\nthis makes it vulnerable to the \"heartbleed\" bug (CVE-2014-0160).\nSee http://heartbleed.com/");	
				$this->SetResult(false);				
			}
			if ($strBuildDate < '2014-06-02')
			{
				$this->Trace("OpenSSL build date is before June 2nd 2014 and\nthis makes it vulnerable to several security issues.\nSee http://www.openssl.org/news/secadv_20140605.txt");	
				$this->SetResult(false);				
			}
			if ($strBuildDate < '2014-06-20')
			{
				$this->Trace('OpenSSL build date is before June 20th 2014.');	
				$this->SetResult(false);				
			}
			if ($strBuildDate < '2015-03-19') 
			{
				$this->Trace("OpenSSL build date is before March 19th 2015.\nIt may still be vulnerable to the\nFREAK attack (CVE-2015-0204).");	 
				$this->SetResult(false);				
			}
			if ($strBuildDate < '2016-05-02') 
			{
				$this->Trace("OpenSSL build date is before May 2nd 2016.\nIt may still be vulnerable to CVE-2016-2107.");	 
				$this->SetResult(false);				
			}
			if ($strBuildDate < '2016-09-22') 
			{
				$this->Trace("OpenSSL build date is before September 22nd 2016.\nIt may still be vulnerable to CVE-2016-6304.");	 
				$this->SetResult(false);				
			}
			if ($strBuildDate < '2016-09-26') 
			{
				$this->Trace("OpenSSL build date is before September 26th 2016.\nIt may still be vulnerable to CVE-2016-6309.");	 
				$this->SetResult(false);				
			}

			/*

			You may use 
			http://filippo.io/Heartbleed/
			or
			http://possible.lv/tools/hb/
			to get an independend opinion on whether a server is vulnerable to the heartbleed bug.
			
			*/

		}
	}
		
