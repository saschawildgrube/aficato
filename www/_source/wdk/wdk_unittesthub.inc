<?php

	error_reporting(E_ALL);
	ini_set('display_errors', 1);

	require_once('wdk_webapplication.inc');
	require_once('wdk_quality.inc');
	require_once('wdk_unittest.inc');
	require_once('wdk_hash.inc');
		
	class CUnitTestHub extends CWebApplication
	{
		function __construct($arrayConfig = array())
		{
			parent::__construct(
				array(),
				$arrayConfig,
				array());
			
			
			$arrayParams = PrepareParameterArray();
			LockParameterSources();
			
			if (ArrayCount($arrayParams) != 1)
			{
				$this->ErrorAndExit('Must have one exactly parameter');
			}
			
			$strTestPath = ArrayGetValue($arrayParams,'test');
			if ($strTestPath == '')
			{
				$this->ErrorAndExit('Test path parameter empty.');
			}
			
			$arrayTestPath = explode('/',$strTestPath);
			if (ArrayCount($arrayTestPath) != 3)
			{
				$this->ErrorAndExit('Invalid test path. Must be like <assembly>/<group>/<testname>');
			}

			$strAssemblyName = $arrayTestPath[0];
			$strGroupName = $arrayTestPath[1];
			$strTestName = $arrayTestPath[2];
			
			$strTestFilePath = $this->MakeTestFilePath(
				$strAssemblyName,
				$strGroupName,
				$strTestName);

			if (!IsFile($strTestFilePath))
			{
				$this->ErrorAndExit('Test file does not exist: "'.$strTestFilePath.'"');
			}
			
			$this->ExecuteTest($strTestFilePath);
		}
		
		function __destruct()
    {
		}
		
		function MakeTestFilePath(
			$strAssemblyName,
			$strGroupName,
			$strTestName)
		{
			return MakeTestFilePath(
				$strAssemblyName,
				$strGroupName,
				$strTestName);
		}
		
		function ExecuteTest($strTestFilePath)
		{
			require_once($strTestFilePath);
			if (!class_exists('CTest'))
			{
				$this->ErrorAndExit('CTest class has not been defined in: "'.$strTestFilePath.'"');
			}
			print('HASH '.MakeHash(FileRead($strTestFilePath))."\n\n");
			$test = new CTest();
		}
		
		function ErrorAndExit($strErrorText)
		{
			print('ERROR: '.$strErrorText."\n");
			print('FAILED');
			exit();	
		}
	}

