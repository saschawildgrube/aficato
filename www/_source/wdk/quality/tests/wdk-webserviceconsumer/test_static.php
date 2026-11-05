<?php

	require_once(GetSourceDir()."webservices_directory.inc");
	require_once(GetWDKDir()."wdk_webserviceconsumer_webapplication.inc");

	class CTest extends CUnitTest
	{

		function __construct()
		{
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();
			parent::__construct("Static webservice transport (requires demo/demo)",$arrayConfig);
		}

		function OnInit()
		{
			$this->RequireWebservice("demo/demo");
			$this->SetVerbose(false);
			return parent::OnInit();
		}

		function Check($bCondition,$strDescription)
		{
			$this->Trace(($bCondition ? 'PASSED: ' : 'FAILED: ').$strDescription);
			if (!$bCondition)
			{
				$this->SetResult(false);
			}
		}

		function OnTest()
		{
			parent::OnTest();
			$this->SetResult(true);
			$strURL = $this->GetWebserviceURL('demo/demo');
			if (GetStaticWebserviceEntryPoint($strURL) === false)
			{
				$this->SetTraceActive(true);
				$this->Trace('PASSED: demo/demo is configured, but its URL does not support local invocation. Static transport checks skipped.');
				return;
			}
			$strServiceURL = preg_replace('~[?#].*$~','',$strURL);
			$strServiceURL = preg_replace('~/index\.php$~','',$strServiceURL);
			$strServiceURL = rtrim($strServiceURL,'/').'/';
			$this->Check(GetStaticWebserviceEntryPoint($strServiceURL.'index.php') !== false,'Resolve explicit index.php');
			$this->Check(GetStaticWebserviceEntryPoint($strServiceURL.'../env.inc') === false,'Reject traversal');
			$this->Check(GetStaticWebserviceEntryPoint('https://invalid.example/webservices/demo/demo/') === false,'Reject foreign host');

			$arrayState = array();
			foreach (array('_GET','_POST','_REQUEST','_COOKIE','_FILES','_SERVER') as $strKey)
			{
				if (array_key_exists($strKey,$GLOBALS))
				{
					$arrayState[$strKey] = $GLOBALS[$strKey];
				}
			}
			$arrayHeaders = headers_list();
			$strWorkingDir = getcwd();
			$strTimezone = date_default_timezone_get();
			$nErrors = error_reporting();
			$strDisplayErrors = ini_get('display_errors');
			$nBufferLevel = ob_get_level();
			$consumer = new CWebServiceConsumerWebApplication($this);
			foreach (array('json','xml','csvpath','json') as $strFormat)
			{
				$arrayParams = array('command' => 'help','format' => $strFormat);
				ob_start();
				print('outer-buffer');
				$bResult = $consumer->ConsumeWebService('demo/demo',$arrayParams,10,'static');
				$strOuterOutput = ob_get_clean();
				$this->Check($bResult,'Repeated static call in '.$strFormat.': '.$consumer->GetError());
				$this->Check($consumer->GetMetaValue('NAME') == 'demo/demo','Response envelope');
				$this->Check($consumer->GetServiceOutput() != '','Serialized response retained');
				$this->Check($strOuterOutput == 'outer-buffer','Caller output buffer preserved');
			}
			$this->Check(!$consumer->ConsumeWebService('demo/demo',array(),10,'invalid')
				&& $consumer->GetError() == 'TRANSPORT_INVALID','Reject invalid transport');
			$this->Check(!$consumer->ConsumeWebService($strServiceURL.'missing/service/',array(),10,'static')
				&& $consumer->GetError() == 'REQUEST_FAILED','Missing entry point fails without HTTP fallback');
			foreach (array('_GET','_POST','_REQUEST','_COOKIE','_FILES','_SERVER') as $strKey)
			{
				$bExists = array_key_exists($strKey,$arrayState);
				$this->Check($bExists === array_key_exists($strKey,$GLOBALS)
					&& (!$bExists || $arrayState[$strKey] === $GLOBALS[$strKey]),'Restore '.$strKey);
			}
			$this->Check(headers_list() === $arrayHeaders,'Restore response headers');
			$this->Check(getcwd() === $strWorkingDir,'Restore working directory');
			$this->Check(date_default_timezone_get() === $strTimezone,'Restore timezone');
			$this->Check(error_reporting() === $nErrors && ini_get('display_errors') === $strDisplayErrors,'Restore error settings');
			$this->Check(ob_get_level() === $nBufferLevel && CWebServiceTransportContext::$nDepth == 0,'Restore context and buffer depth');
		}
	}
