<?php

	require_once(GetWDKDir()."wdk_ip.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test GetHostNameFromIP");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}

		function TestCase_GetHostNameFromIP(
			$strIP,
			$vExpectedValue)
		{ 
			$this->Trace("TestCase_GetHostNameFromIP");
		
			$vResult = GetHostNameFromIP($strIP);
		
			$this->Trace("GetHostNameFromIP(".RenderValue($strIP).") = ".RenderValue($vResult));
		
			if ($vResult === $vExpectedValue)
			{
				$this->Trace("Testcase PASSED!");
			}
			else
			{
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);	
			}
			$this->Trace("");
			$this->Trace("");
		}

		
		function OnTest()
		{
			parent::OnTest();

			// Example.com resolves to 93.184.216.34. But 93.184.216.34 does not allow reverse lookup.
			$this->TestCase_GetHostNameFromIP("93.184.216.34",false);
			
			// Invalid IP addresses
			$this->TestCase_GetHostNameFromIP("",false);
			$this->TestCase_GetHostNameFromIP("0.0.0.0",false);
			$this->TestCase_GetHostNameFromIP("bogus",false);
			
			// DENIC			
			$this->TestCase_GetHostNameFromIP("81.91.170.12","www.denic.de");
			
		}
		
		
	}
	
	

		
