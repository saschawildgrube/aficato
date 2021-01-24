<?php
	
	require_once(GetWDKDir()."wdk_unittest.inc");
	require_once(GetWDKDir()."wdk_wget.inc");
		
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("wget vulnerabilities");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
		
		function OnTest()
		{
			parent::OnTest();
		
			$this->Trace("");
		
			$arrayVersion = GetVersionInfoWget();
			$strVersion = ArrayGetValue($arrayVersion,"VERSION");
			$strVerboseVersion = ArrayGetValue($arrayVersion,"VERBOSE");
			$this->Trace("wget version: ".$strVersion);
			$this->Trace("wget verbose version: ".$strVerboseVersion);
			if (CompareVersion($strVersion,"1.16") == -1)
			{
				if (CompareVersion($strVersion,"1.13.4") == 0)
				{
					if (		$strVerboseVersion != "1.13.4-2ubuntu1.2"
							&&	$strVerboseVersion != "1.13.4-2ubuntu1.3"
							&&	$strVerboseVersion != "1.13.4-2ubuntu1.4")
					{
						$this->Trace("FAILED: wget version is 1.13.4 but package is not 1.13.4-2ubuntu1.2, 1.13.4-2ubuntu1.3, 1.13.4-2ubuntu1.4");
						$this->Trace("Unpatched wget versions prior to 1.16 are vulnerable to CVE-2014-0160.");	
						$this->Trace("Refer to: http://www.cvedetails.com/cve/CVE-2014-4877/");	
						$this->SetResult(false);				
					}
				}
				else
				{
					$this->Trace("FAILED: wget version is 1.15 or earlier.");
					$this->Trace("wget versions prior to 1.16 are vulnerable to CVE-2014-0160.");	
					$this->Trace("Refer to: http://www.cvedetails.com/cve/CVE-2014-0160/");	
					$this->SetResult(false);		
				}		
			}
		}
	}
		
