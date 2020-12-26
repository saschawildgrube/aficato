<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('MakeHash');
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_MakeHash($vPayload,$strSeed,$strMethod,$strExpectedResult)
		{
			$this->Trace('TestCase_MakeHash');
			$this->Trace('vPayload        : '.RenderValue($vPayload));
			$this->Trace('strSeed         : '.RenderValue($strSeed));
			$this->Trace('strMethod       : '.RenderValue($strMethod));
			$this->Trace('Expected Result : '.RenderValue($strExpectedResult));
			$strResult = MakeHash($vPayload,$strSeed,$strMethod);
			$this->Trace('MakeHash returns: '.RenderValue($strResult));
			if ($strResult == $strExpectedResult) 
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
			
			$this->TestCase_MakeHash('','',null,				'da39a3ee5e6b4b0d3255bfef95601890afd80709');
			$this->TestCase_MakeHash(false,false,null,	'da39a3ee5e6b4b0d3255bfef95601890afd80709');
			
			$this->TestCase_MakeHash('Payload','seed',null,		'7fc26027a912c7dfabbd9f4ee36defd6b2b52590');
			$this->TestCase_MakeHash('Payload','seed','sha1',	'7fc26027a912c7dfabbd9f4ee36defd6b2b52590');
			
			$this->Trace('Try different methods:');
			$this->TestCase_MakeHash('Payload','seed','bcrypt7',	'8de25a127f4b242a5794b5320949687947dd34ba');
			$this->TestCase_MakeHash('Payload','seed','bcrypt12',	'4b8b64c9f4776ad73b43f1c287c3f827a56b53e3');

			$this->Trace('Try different payloads:');
			$this->TestCase_MakeHash('Other payload','seed',null,			'749a0626c151f371362478e9428df9bf3f3d5a8d');
			
			$this->Trace('Try different seeds:');
			$this->TestCase_MakeHash('Payload','different seed',null,	'89e7a791775c67f86d8d6301e39332326b84f7b4');

			$this->Trace('Try different input types:');
			$this->TestCase_MakeHash('123','456',null,	'f18f057ea44a945a083a00e6fcc11637d186042d');
			$this->TestCase_MakeHash(123,'456',null,		'f18f057ea44a945a083a00e6fcc11637d186042d');
			$this->TestCase_MakeHash('123',456,null,		'f18f057ea44a945a083a00e6fcc11637d186042d');
			$this->TestCase_MakeHash(123,456,null,			'f18f057ea44a945a083a00e6fcc11637d186042d');
			
			$this->Trace('Try array payloads:');
			$arrayPayload1 = array(
				'key1' => 'hello',
				'key2' => 'world');
			$arrayPayload2 = array(
				'hello',
				'world');
			$arrayPayload3 = array(
				111 => 'hello',
				999 => 'world');
			$arrayPayload4 = array(
				111 => 'hello',
				555 => '',
				999 => 'world');			
			$this->TestCase_MakeHash($arrayPayload1,'seed',null,'b8c0e63898ca42a0a41aaefc3a4aeb1311439cd1');
			$this->TestCase_MakeHash($arrayPayload2,'seed',null,'b8c0e63898ca42a0a41aaefc3a4aeb1311439cd1');
			$this->TestCase_MakeHash($arrayPayload3,'seed',null,'b8c0e63898ca42a0a41aaefc3a4aeb1311439cd1');
			$this->TestCase_MakeHash($arrayPayload4,'seed',null,'b8c0e63898ca42a0a41aaefc3a4aeb1311439cd1');
			// yes, keys names or indices are ignored. Empty values are ignored, too. Only non-empty content counts.
			
			$this->Trace('Try array of arrays:');
			$arrayPayload1 = array(
				array('alpha','beta'),
				array('Joe','Jane'));
			$arrayPayload2 = array(
				array('gamma','beta'),
				array('Joe','Jane'));
			$this->TestCase_MakeHash($arrayPayload1,'seed',null,'285c7d0390e4208c3a5123109af4ae09d9aa3d86');
			$this->TestCase_MakeHash($arrayPayload2,'seed',null,'cfddadd99e9467fbd9a9468008744f6ff1b11f3f');
			


		}
		

	}
	
	

		
