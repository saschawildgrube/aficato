<?php

	require_once(GetWDKDir().'wdk_module.inc');
	require_once(GetWDKDir().'wdk_httpupload.inc');
	
	class CAMLModule extends CModule
	{
		
		private $m_strMandateID;
	
		function __construct(
			CWebSite $website,
			$strModulePath,
			$strInstanceID = '',
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				$strModulePath,
				$strInstanceID,
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			$this->m_strMandateID = false;
			return true;
		}
		
	
		function CallbackRenderModule($strState)
		{
			$strMandateID = $this->GetUserMandateID();
			if ($strMandateID == false)
			{
				$this->SetErrorMessageCode('NOMANDATE');
				return 'error';	
			}
			
			$this->m_strMandateID = $strMandateID;
			
			return '';
		}
		
	
		
		
		
		
		function GetUserMandateID()
		{
			$strUserID = $this->GetWebsite()->GetAuthUserID();	
			
			if ($strUserID == '')
			{
				return false;
			}
			
			$strMandateID = $this->GetWebsite()->GetData('USER',$strUserID,'mandate','');
			
			if ($strMandateID == '')
			{
				return false;	
			}
			
			return $strMandateID;
		}
		
		function GetMandateDataDir($strMandateID)
		{
			if ($strMandateID == '')
			{
				return false;	
			}
			$strMandateDataDir = GetDocumentRootDir();
	 		$strMandateDataDir .= '../../';
	 		
 			if (GetEnvID() == 'dev')
 			{
 				$strMandateDataDir .= 'data-dev/';	
 			}
 			else
 			{
 				$strMandateDataDir .= 'data/';
 			}
 			$strMandateDataDir .= 'mandates/'.$strMandateID.'/';
 
 			return $strMandateDataDir;		
			
		}

		function GetAccountDataDir()
		{
			$strMandateDataDir = $this->GetMandateDataDir($this->m_strMandateID);
			if ($strMandateDataDir == '')
			{
				return false;	
			}
			$strMandateDataDir .= 'accounts/'; 
 			
 			return $strMandateDataDir;		
		}

		
		function GetAccountDataFiles()
		{
			$arrayAccountDataFiles = array();
			
			$strAccountDataDir = $this->GetAccountDataDir();
			if ($strAccountDataDir == false)
			{
				return false;	
			}
			$arrayFiles = ReadDirectory($strAccountDataDir);
			if ($arrayFiles === false)
			{
				return false;	
			}
 			
			if (ArrayCount($arrayFiles) != 0)
			{
				foreach ($arrayFiles as $strFile)
				{
					if (GetExtentionFromPath($strFile) != 'csv')
					{
						continue;
					}
					$arrayAccountDataFiles[] = $strFile;
				}
			}
			
			$arrayAccountDataFiles = ArraySort($arrayAccountDataFiles,false,'CompareString');
			return $arrayAccountDataFiles;	
		}


	}

