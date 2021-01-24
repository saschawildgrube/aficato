<?php
	
	require_once(GetWDKDir().'wdk_unittest_recursivefilecheck.inc');
	
	class CTest extends CUnitTestRecursiveFileCheck
	{
		function __construct()
		{
			parent::__construct('Check for deprecated functions and classes in php source code');
		}
		 
		
		
		function OnTestCaseCheckFile($strFilePath)
		{ 
			$arrayRegExp = array();
			$strExtention = GetExtentionFromPath($strFilePath);
			if (	$strExtention == 'inc'
				||  $strExtention == 'php')
			{
				$strFileName = GetFilenameFromPath($strFilePath);
				// we don't want to fail the test because of THIS file!
				if ($strFileName == 'test_deprecated.php')
				{
					return;	
				}


				$arrayRegExp = array
					(
						'/[^gy]CheckCharSet\(/',
						'/CContentDefault/',
						'/CUsersWebsite/',
						'/RenderFormGeneric/',
						'/->Log\(/',
						'/->Event\(/',
						'/(=\s*|\.\s*|return\s+|=>\s*|\(\s*)GetWebserviceURL\s*\(/',
						'/"databasesupport"/',
						'/"maxcontentsize"/',
						"/\"nolog\"/",
						"/\"noevents\"/",
						"/\"maxtimeout\"/",
						"/\[\"statushistory\"\]/",
						"/\"remotesites\"/",
						"/\"requesttimeout\"/",
						"/\"maxattempts\"/",
						"/\"maxseconds\"/",
						"/\"maxtests\"/",
						"/\"retryonrequesterror\"/",
						'/\["media"\]/',
						"/\[\"doctype\"\]/",
						"/MakeLink\(/",
						"/ReadFile"."CSV/",
						"/GetDataArrayFromCSV\(/",
						"/\"session_support\"/",
						"/MakeSQL_PrefixTableNamesInQuery(/",
						"/\tTestURL\(/",
						"/\tMonitorURL\(/",
						"/TestServer\(/",
						"/TestDomain\(/",
						"/CUnitTestURL/",
						"/CUnitTestDomain/",
						"/CUnitTestServer/",
						"/wdk_unittest_url.inc/",
						"/wdk_unittest_domain.inc/",
						"/wdk_unittest_server.inc/",
						'/ xmlns="http://www.w3.org/1999/xhtml"/',
						"/GetBoolString/",
						'"assembly_blacklist"/',
						'/VerifyTextID/',
						'/GetInvalidTextIDs/',
						'/wdk_dns.inc/',
						'/mysql_/',
						'/MakeSQL_Assignment\(/',
						"/MakeSQL_Value\(/",
						"/MakeSQL_Field\(/",
						"/MakeSQL_Fields\(/",
						"/MakeSQL_Table\(/",
						"/MakeSQL_EscapeString\(/",
						"/MakeSQL_ConditionalExpression\(/",
						"/CallbackCheckParam\(/",
						'/"phpwarning"/',
						'/CallbackRenderImage\(/',
						'/function CallbackCheckInputValues\(\$strEntity,\$bAdd,\&\$arrayItem,\&\$arrayErrors\)/',
						'/function CallbackPrepareItemDataExport\(\$strEntity,\$strItemIndex,\&\$arrayItem\)/',
						'/function CallbackPrepareItemDataImport\(\$strEntity,\$bAdd,\&\$arrayItem\)/',
						'/"tidwarning"/',
						'/unintendedoutputcheck_defuse/',
						'/"maxcontentsize"/',
						'/\-\>IncludeElement\((\"|\')link(\"|\')\)/',
						'/\-\>IncludeElement\((\"|\')linkicon(\"|\')\)/',
						'/\-\>IncludeElement\((\"|\')list(\"|\')\)/',
						'/\-\>IncludeElement\((\"|\')navigation(\"|\')\)/',
						'/\-\>IncludeElement\((\"|\')message(\"|\')\)/',
						'/\-\>IncludeElement\((\"|\')messagestack(\"|\')\)/',
						'/\-\>IncludeElement\((\"|\')form(\"|\')\)/',
						'/\-\>IncludeElement\((\"|\')toolbar(\"|\')\)/',
						'/\-\>IncludeElement\((\"|\')table(\"|\')\)/',
						'/IsFeatureDisabled\(/',
						'/CHARSET_ALPHALOWERCASE/',
						'/CHARSET_ALPHAUPPERCASE/',
						'/CHARSET_ALPHANUMERICUPPERCASE/',
						'/CHARSET_ALPHANUMERICLOWERCASE/',
						'/\<script language=\"PHP\"\>/',
						'/language="javascript"/', // https://support.google.com/adwords/answer/1722021?hl=de
						'/check_unintendedoutput_defuse/',
						'/E_STRICT/',
						'/TraceArray/',
						'/jsonpretty/',
						'/wdk-progressindicator(\"|\')/',
						'/wdk-progressindicator-download(\"|\')/',
						'/RedirectAtOnce/',
						'/MakeProtocolLink/',
						'/ _construct()/',
						'/linkexternal/',
						'/listdetails/',
						'/filecategory-archive/',
						'/filecategory-folder/',
						'/filecategory-image/',
						'/filecategory-other/',
						'/filecategory-pdf/',
						'/filecategory-text/',
						'/filecategory-web/',
						'/linkstrong/',
						'/notok/',
						'/\-\>IncludeElement\((\"|\')carousel(\"|\')\)/',
						'/\-\>IncludeModule\((\"|\')security\/ssl(\"|\')\);/',
						'/CCountdownElement/',
						'/RenderCountdown/(/',
						'/IsDevice/',
						'/GetDatabaseConfigID/(/',
						'/GetStage\(/',
						'/CBootstrapLayout/',
						'/CBootstrapThemeLayout/',
						'/IncludeAssembly\(/',  
						'/RegisterAssembly\(/',
						'/On_TestCase_CheckFile\(/',
						'/On_TestCase_CheckFolder\(/',
						'/::Callback/',
						'/m_bCallback/'
					);
					
				if ($strFileName != 'wdk_url.inc')
				{
					$arrayRegExp[] = '/urlencode\(/';
				}

				if ($strFileName != 'wdk_element.inc')
				{
					$arrayRegExp[] = '/$this->m_strLayoutHTML;/';  
				}
					
			}
			else if (	$strExtention == 'htm'
						||  $strExtention == 'cfg')
			{
				$arrayRegExp = array
					(
						' xmlns=\"http://www.w3.org/1999/xhtml\"',
						'/{MODULE_SSL_ICON}/'		
					);				
			}
			$this->CheckFileAgainstRegExp($strFilePath,$arrayRegExp);
		}

		function OnTest()
		{
			parent::OnTest();
			$this->SetResult(true);
			$this->CheckSourceDirectories();
		}
	}
