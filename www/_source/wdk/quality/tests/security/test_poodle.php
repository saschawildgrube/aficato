<?php
	
	require_once(GetWDKDir()."wdk_unittest.inc");
		
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Poodle Vulnerability CVE-2014-3566");
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
		
			$this->Trace("Checking the Domain for the Poodle vulnerability (CVE-2014-3566)...");
			$this->Trace("For more information go to:");
			$this->Trace("http://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2014-3566");
			$this->Trace("http://www.poodlescan.com/ (Vulnerability test for servers)");
			$this->Trace("https://www.poodletest.com/  (Vulnerability test for browsers)");
			$this->Trace("https://www.openssl.org/~bodo/ssl-poodle.pdf");
			$this->Trace("https://zmap.io/sslv3/");
			$this->Trace("");
		
		
			
		
			$strURL = "https://".GetRootURL();
			$this->Trace("$strURL");
			
			$this->Trace("");
			$this->Trace("Check SSLv3");

			$ch = curl_init($strURL);
			curl_setopt($ch, CURLOPT_TIMEOUT, 5);
			curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
			curl_setopt($ch, CURLOPT_HEADER, true);
			curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
			curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);
			curl_setopt($ch, CURLOPT_SSLVERSION, 3);
			$strOutput = curl_exec($ch);
			$nError = curl_errno($ch);
			$strError = curl_error($ch);
			curl_close($ch);
			
			$this->Trace("Error: ".$strError." (".$nError.")");
			
			if ($nError == 35)
			{
				$this->Trace("PASSED");
				$this->Trace("The request failed due to SSL connection error.");
			}
			else if ($nError == 4)
			{
				$this->Trace("PASSED");
				$this->Trace("No SSLv3 support on the server.");
			}
			else if ($nError == 0)
			{
				$this->Trace("FAILED");
				$this->Trace("The request should not be working when demanding SSLv3.");
				$this->SetResult(false);
			}
			else
			{
				$this->Trace("UNDEFINED");
				$this->Trace("The request failed due to other reasons. So there is no indication that SSLv3 is NOT supported.");
			}
			




			$this->Trace("");
			$this->Trace("Check SSLv2");

			$ch = curl_init($strURL);
			curl_setopt($ch, CURLOPT_TIMEOUT, 5);
			curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
			curl_setopt($ch, CURLOPT_HEADER, true);
			curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
			curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);
			curl_setopt($ch, CURLOPT_SSLVERSION, 2);
			$strOutput = curl_exec($ch);
			$nError = curl_errno($ch);
			$strError = curl_error($ch);
			curl_close($ch);
			
			$this->Trace("Error: ".$strError." (".$nError.")");
			
			if ($nError == 35)
			{
				$this->Trace("PASSED");
				$this->Trace("The request failed due to SSL connection error.");
			}
			else if ($nError == 4)
			{
				$this->Trace("PASSED");
				$this->Trace("No SSLv2 support on the server.");
			}
			else if ($nError == 0)
			{
				$this->Trace("FAILED");
				$this->Trace("The request should not be working when demanding SSLv2.");
				$this->SetResult(false);
			}
			else
			{
				$this->Trace("UNDEFINED");
				$this->Trace("The request failed due to other reasons. So there is no indication that SSLv2 is NOT supported.");
				//$this->SetResult(false);
			}

		
		
		
		
		
			
		}
	}
		
