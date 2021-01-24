<?php
	
	require_once(GetWDKDir().'wdk_unittest_monitoring.inc');
	
	class CTest extends CMonitoringUnitTest
	{
		function __construct()
		{
			parent::__construct('W3C Validator');
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
			
			$this->TestCase_w3cValidateURL(
				'http://'.GetRootURL(),
				'PASSED');

			$this->TestCase_w3cValidateURL(
				'http://'.GetRootURL().'?trace=1',
				'PASSED');
		}

	}
	
	

		
