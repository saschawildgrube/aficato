<?php

	function IsFile($strFilePath, $bUseShell = false)
	{
		$bResult = false;
		if ($bUseShell == true)
		{
			require_once(GetWDKDir().'wdk_shell.inc');
			$arrayResult = array();
			$bResultSE = ShellExecute(
				'[ -f "'.$strFilePath.'" ]',
				array(),
				array(),
				'',
				$arrayResult
				);
			if ($bResultSE == false)
			{
				return IsFile($strFilePath,false);	
			}
			if ($arrayResult['return'] == 0)
			{
				return true;	
			}
		}
		else
		{
			$ea = new CErrorAmnesty();
			$ea->Start();			
			$bResult = is_file($strFilePath);	
			$ea->End();
		}
		return $bResult;
	}
	
	function IsDirectory($strDirPath)
	{
		if ($strDirPath == "/")
		{
			return true;	
		}
		if (StringSection($strDirPath,-1,1) != "/")
		{
			return false;
		}
		if (FindString($strDirPath,"//") != -1)
		{
			return false;	
		}
		$ea = new CErrorAmnesty();
		$ea->Start();
		$bResult = is_dir($strDirPath);
		$ea->End();
		
		return $bResult;
	}
	
	function IsDirectoryReadWriteAccess($strDirPath)
	{
		if (IsDirectory($strDirPath) == false)
		{
			return false;	
		}
		$strFilePath = MakeUniqueTempFilePath($strDirPath);
		$bResult = FileAddText($strFilePath,"test");
		if (IsFile($strFilePath))
		{
			DeleteFile($strFilePath);
			return true;	
		}
		return false;
	}
	
	function DeleteFile($strFilePath,$bUseShell = false)
	{
		if (!IsFile($strFilePath,$bUseShell))
		{
			return true;
		}
		if ($bUseShell == true)
		{
			require_once(GetWDKDir().'wdk_shell.inc');
			$arrayResult = array();
			$bResultSE = ShellExecute(
				'rm '.$strFilePath,
				array(),
				array(),
				'',
				$arrayResult
				);
			if ($bResultSE == false)
			{
				return DeleteFile($strFilePath,false);	
			}
			if (!IsFile($strFilePath,true))
			{
				return true;
			}
		}
		else
		{
			unlink($strFilePath);
			if (!IsFile($strFilePath))
			{
				return true;
			}
		}
		return false;	
	}

	function ReadDirectory($strDir, $bRecursive = false, $bFullPath = false)
	{
		if (!IsDirectory($strDir))
		{
			return false;
		}
		$strFullPathPrefix = ($bFullPath)?($strDir):("");
		$arrayFolders = array();
		$arrayFiles = array();
		
		$ea = new CErrorAmnesty();
		$ea->Start();
		$directory = dir($strDir);
		$ea->End();
		
		if ($directory == false)
		{
			return false;	
		}

		while (false !== ($strEntry = $directory->read()))
		{ 
			if ($strEntry != '.' && $strEntry != '..')
			{
				$strSubDir = $strDir.$strEntry."/";
				if (IsDirectory($strSubDir))
				{
					$arrayFolders[] = $strEntry."/";
					if ($bRecursive == true)
					{
						$arraySubDir = ReadDirectory($strSubDir,true,$bFullPath);
						if ($arraySubDir != false)
						{
							if ($bFullPath == false)
							{
								foreach ($arraySubDir as &$strSubDirEntry)
								{
									$strSubDirEntry = $strEntry."/".$strSubDirEntry;
								}
			   			}
							$arrayFolders = ArrayAppend($arrayFolders,$arraySubDir);
						}
					}
				}
		   	else
		   	{
					$arrayFiles[] = $strFullPathPrefix.$strEntry;	
		   	}
			}
		}
		$arrayFolders = ArraySort($arrayFolders);
		$arrayFiles = ArraySort($arrayFiles);
		$arrayDir = ArrayAppend($arrayFolders,$arrayFiles);
		return $arrayDir;
	}
	
	function GetDirectoryFromPath($strPath)
	{
		$pos = mb_strrpos($strPath,"/");
		if ($pos === false)
		{
			return false;	
		}
		
		$strDirectory = StringSection(
			$strPath,
			0,
			1+$pos);
		return $strDirectory;	
	}
	
	function GetParentDirectoryFromPath($strPath)
	{
		$strPath = GetDirectoryFromPath($strPath);
		if ($strPath == false)
		{
			return false;	
		}
		$strPath = StringSection($strPath,0,-1);
		$strPath = GetDirectoryFromPath($strPath);
		return $strPath;
	}
	
	function GetExtentionFromPath($strPath)
	{
		$nSlashPos = mb_strrpos($strPath,"/");
		if ($nSlashPos === false)
		{
			$nSlashPos = -1;	
		}
		$nDotPos = mb_strrpos($strPath,".");
		if ($nDotPos === false)
		{
			return "";
		}
		if ($nSlashPos > $nDotPos) // file name does not contain an extention
		{
			return "";	
		}
		return StringSection($strPath,$nDotPos+1);
	}

	function GetFileNameFromPath($strPath)
	{
		$nSlashPos = mb_strrpos($strPath,"/");
		if ($nSlashPos === false)
		{
			return $strPath;
		}
		return StringSection($strPath,$nSlashPos+1);
	}
	
	function GetFileTitleFromPath($strPath)
	{
		$strFileName = GetFileNameFromPath($strPath);
		$nDotPos = mb_strrpos($strFileName,".");
		if ($nDotPos === false || $nDotPos === 0)
		{
			return $strFileName;
		}
		return StringSection($strFileName,0,$nDotPos);
	}


	function GetFolderNameFromPath($strPath)
	{
		$strPath = GetDirectoryFromPath($strPath);
		if ($strPath === false)
		{
			return "";	
		}
		
		$nSlashPos = mb_strrpos($strPath,"/");
		if ($nSlashPos === false)
		{
			return $strPath;
		}
		$strPath = StringSection($strPath,0,$nSlashPos);
		$nSlashPos = mb_strrpos($strPath,"/");
		if ($nSlashPos === false)
		{
			return $strPath;
		}
		return StringSection($strPath,$nSlashPos+1);
	}
	
	
	/*
		filesize() and stat() won't work due to maxint contraints
		on files larger than 2GB. Reading the size from the os'
		stat shell command however works.
	*/
	function GetFileSize($strFilePath, $bUseShell = false)
	{
		if (!IsFile($strFilePath,$bUseShell))
		{
			return false;	
		}
		require_once("wdk_shell.inc");
		$arrayResult = array();
		ShellExecute(
			"stat",
			array("--format=%s",$strFilePath),
			array(),
			"",
			$arrayResult);
		$strStdOut = ArrayGetValue($arrayResult,"stdout");
		return floatval($strStdOut);
	}
	
	function GetFileLastChangeTime($strFilePath)
	{
		if (!IsFile($strFilePath))
		{
			return false;	
		}
		return filemtime($strFilePath);	
	}

	function MakeUniqueTempFilePath($strDirPath)
	{
		if (!IsDirectory($strDirPath))
		{
			return false;	
		}
		$strFilePath = "";
		do 
		{
			$strFileName = MakeNumericToken(20).".tmp";
			$strFilePath = $strDirPath . $strFileName;
		} while (IsFile($strFilePath));
		
		return $strFilePath;		
	}

	function GetDiskSpaceTotal($strDir)
	{
		$ea = new CErrorAmnesty();
		$ea->Start();
		$nBytes = disk_total_space($strDir);
		$ea->End();
		return $nBytes;
	}
	
	function GetDiskSpaceFree($strDir)
	{
		$ea = new CErrorAmnesty();
		$ea->Start();
		$nBytes = disk_free_space($strDir);
		$ea->End();
		return $nBytes;
	}	
	function GetDiskSpaceUsed($strDir)
	{
		return GetDiskSpaceTotal($strDir) - GetDiskSpaceFree($strDir);
	}
	
	function IsValidPath($strPath)
	{
		if ($strPath == '')
		{
			return false;	
		}
		if (StringCheckCharSet($strPath,CHARSET_PATH) == false)
		{
			return false;
		}
		if (FindString($strPath,"//") != -1)
		{
			return false;	
		}
		return true;	
	}
	
	function IsFilePath($strPath)
	{
		if (IsValidPath($strPath) == false)
		{
			return false;	
		}
		if (StringSection($strPath,-1,1) == '/') // Looks like a folder
		{
			return false;	
		}
		return true;
	}
	
	function IsDirectoryPath($strPath)
	{
		if (IsValidPath($strPath) == false)
		{
			return false;	
		}
		if (StringSection($strPath,-1,1) == '/') // Looks like a folder
		{
			return true;	
		}
		return false;
	}
	
	function PathAddTrainingSlash($strPath)
	{
		if (StringSection($strPath,-1,1) !=  "/")
		{
			$strPath.="/";
		}
		return $strPath;
	}
	
	function GetFileCategory($strPath)
	{
		if (StringSection($strPath,-1,1) == '/')
		{
			return 'folder';	
		}
		$strExt = GetExtentionFromPath($strPath);
		switch ($strExt)
		{
		case 'txt':
		case 'cfg':
			return 'text';
		case 'pdf':
			return 'pdf';
		case 'png':
		case 'gif':
		case 'bmp':
		case 'tif':
		case 'jpg':
		case 'svg':
			return 'image';
		case 'html':
		case 'htm':
		case 'css':
		case 'map':
		case 'php':
		case 'inc':
		case 'js':
		case 'xml':
			return 'code';
		case 'mp4':
		case 'mpeg':
		case 'ogv':
		case 'webm':
			return 'video';
		default:
			return 'other';	
		}	
	}
	
	function GetAllMimeTypes()
	{
		// Based on http://www.webmaster-toolkit.com/mime-types.shtml
		require_once('wdk_csv.inc');
		$arrayMimeTypes = FileReadCSV_RFC4180(GetWDKDir().'data/mimetypes.csv');
		return $arrayMimeTypes;
	}
	
	function GetMimeTypeFromPath($strPath)
	{
		$strExt = GetExtentionFromPath($strPath);
		//print($strExt);
		if ($strExt == "")
		{
			return false;	
		}
		$arrayMimeTypes = GetAllMimeTypes();
		if (!is_array($arrayMimeTypes))
		{
			return false;	
		}
		foreach ($arrayMimeTypes as $arrayMimeType)  
		{
			if (ArrayGetValue($arrayMimeType,"EXTENSION") == $strExt)
			{
				return ArrayGetValue($arrayMimeType,"MIME-TYPE");	
			}	
		}
		return false;
	}


