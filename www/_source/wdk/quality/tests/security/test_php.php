<?php
	
	require_once(GetWDKDir()."wdk_unittest.inc");
	require_once(GetWDKDir()."wdk_php.inc");
		
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("php vulnerabilities");
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
		
			$this->Trace("This checks against php version which are immune against CVE-2014-3668, CVE-2014-3669 and CVE-2014-3670.");
			$this->Trace("Refer to http://php.net/archive/2014.php#id2014-10-16-2");
			$this->Trace("");
		
			$arrayVersion = GetVersionInfoPHP();
			$strVersion = ArrayGetValue($arrayVersion,"VERSION");
			$this->Trace("Check PHP version: ".$strVersion);
			$this->SetResult(false);
			if (CompareVersion("5.4.33",$strVersion) == -1)
			{
				$this->Trace("OK: PHP version is 5.4.34 or higher");
				$this->SetResult(true);															
			}
			
			if ($this->GetResult() == false)
			{
				$this->Trace("FAILED: No supported and secure PHP version!");	
			}
			

		}
	}
		
