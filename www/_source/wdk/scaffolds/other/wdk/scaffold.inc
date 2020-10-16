<?php

	class CMyScaffolder extends CScaffolder
	{
		
		function IsConfigRequired()
		{
			return false;	
		}

		function CallbackPrepareForm(CFormDefinition &$formDef)
		{
		}
		
		function CallbackGetParams(&$arrayParams)
		{
		}
		
		function CallbackCheckParams($arrayParams,&$arrayErrors)
		{
		}
		
		function CallbackAddAdditionalArchiveFiles($arrayParams,&$arrayArchiveFiles)
		{
			$arrayFiles = ReadDirectory(GetWDKDir(),true);
			foreach ($arrayFiles as $strFilePath)
			{
				if (StringSection($strFilePath,-1,1) != "/")	
				{
					$arrayArchiveFiles[] = array
						(
						"archivefilepath" => "wdk/".$strFilePath,
						"filepath" => GetWDKDir() . $strFilePath
						);	
				}
			}
		}


	}
	
