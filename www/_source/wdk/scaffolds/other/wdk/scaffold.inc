<?php

	class CMyScaffolder extends CScaffolder
	{
		
		function IsConfigRequired()
		{
			return false;	
		}

		function OnPrepareForm(CFormDefinition &$formDef)
		{
		}
		
		function OnGetParams(&$arrayParams)
		{
		}
		
		function OnCheckParams($arrayParams,&$arrayErrors)
		{
		}
		
		function OnAddAdditionalArchiveFiles($arrayParams,&$arrayArchiveFiles)
		{
			$arrayFiles = ReadDirectory(GetWDKDir(),true);
			foreach ($arrayFiles as $strFilePath)
			{
				if (StringSection($strFilePath,-1,1) != '/')	
				{
					$arrayArchiveFiles[] = array
						(
						'archivefilepath' => 'wdk/'.$strFilePath,
						'filepath' => GetWDKDir() . $strFilePath
						);	
				}
			}
		}


	}
	
