<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('VerifyPassword');
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_VerifyPassword($strPassword,$strPasswordHash,$strPasswordHashSeed,$bExpectedResult)
		{
			$this->Trace('TestCase_VerifyPassword');
			$this->Trace('strPassword = "'.$strPassword.'"');
			$this->Trace('strPasswordHashSeed = "'.$strPasswordHashSeed.'"');
			$this->Trace('strPasswordHash = "'.$strPasswordHash.'"');
			$this->Trace('Expected Result: '.RenderBool($bExpectedResult));
			$bResult = VerifyPassword($strPassword,$strPasswordHash,$strPasswordHashSeed);
			$this->Trace('VerifyPassword returns: '.RenderBool($bResult));
			if ($bResult == $bExpectedResult) 
			{
				$this->Trace('Testcase PASSED!');
			}
			else
			{
				$this->Trace('Testcase FAILED!');	
				$this->SetResult(false);
			}
			$this->Trace('');
			
		}
		
		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->Trace('Random data using the default behaviour');
			$strPassword = '123';
			$strPasswordHashSeed = MakePasswordHashSeed(); 
			$strPasswordHash = MakePasswordHash($strPassword,$strPasswordHashSeed);
			$this->TestCase_VerifyPassword($strPassword,$strPasswordHash,$strPasswordHashSeed,true);
			$this->TestCase_VerifyPassword('1234',$strPasswordHash,$strPasswordHashSeed,false);
			
			$this->Trace('Static data with an sha1 hash');
			$this->TestCase_VerifyPassword('123','178b199d75c26c4d32b30f09590ef2c29df3f162','01a0f48bc105f98f86f653d7d514c9e83f62e417',true);
			$this->TestCase_VerifyPassword('1234','178b199d75c26c4d32b30f09590ef2c29df3f162','01a0f48bc105f98f86f653d7d514c9e83f62e417',false);

			$this->Trace('Static data with a bcrypt hash');
			$this->TestCase_VerifyPassword('123','19029b9be0eae609c425b8e3232f2fa3a52b7e7e','f93f09112978daec919a40a7c38c4c560a4d0791',true);
			$this->TestCase_VerifyPassword('1234','e616d4e19f8148f60195fd389b038b12cc6aa298','debe3bf08b012f2b16371141efd7362d13186e88',false);
 
			$this->Trace('This must fail obviously');
			$this->TestCase_VerifyPassword('123','','',false);

		}
		

	}
	
	

		
