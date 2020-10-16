<?php
	
	function CompressFiles($strZipFilePath, $arrayFiles, $strArchiveComment = '')
	{
		if (!is_array($arrayFiles))
		{
			return false;	
		}
		if (IsFile($strZipFilePath))
		{
			return false;	
		}
		$strFileDir = GetDirectoryFromPath($strZipFilePath);
		if (!IsDirectory($strFileDir))
		{
			return false;	
		}
		
		$zip = new ZipArchive();
		if ($zip->open($strZipFilePath, ZIPARCHIVE::CREATE) !== true)
		{
    	return false;
		}
		
		if ($strArchiveComment != '')
		{
			$zip->setArchiveComment($strArchiveComment);	
		}

		foreach ($arrayFiles as $arrayFileInfo)
		{
			$strArchiveFilePath =	ArrayGetValue($arrayFileInfo,'archivefilepath');
			if (!IsValidPath($strArchiveFilePath))
			{
				$zip->unchangeAll();
				return false;	
			}
			
			
			$strArchiveFileComment =	ArrayGetValue($arrayFileInfo,'archivefilecomment');
			
			$strContents =	ArrayGetValue($arrayFileInfo,'contents');
			$strFilePath =	ArrayGetValue($arrayFileInfo,'filepath');
			if ($strContents != '' && $strFilePath != '')
			{
				$zip->unchangeAll();
				return false;	
			}

			if ($strFilePath != '')
			{
				if (!IsFile($strFilePath))
				{
					$zip->unchangeAll();
					return false;	
				}
				$zip->addFile($strFilePath,$strArchiveFilePath);
			}
			else
			{
				$zip->addFromString($strArchiveFilePath, $strContents);
			}
			if ($strArchiveFileComment != '')
			{
				$zip->setCommentName($strArchiveFilePath, $strArchiveFileComment);				
			}
		}
		return $zip->close();
	}


	function CompressFilesInMemory($arrayFiles, $strArchiveComment = '')
	{
		$strTempDir = GetTempDir();
		$strTempFile = $strTempDir.MakeNumericToken(10);
		
		$bResult = CompressFiles($strTempFile,$arrayFiles, $strArchiveComment);
		$strArchiveFileContents = FileReadBinary($strTempFile);
		DeleteFile($strTempFile); 
		if ($bResult == false)
		{
			return false;	
		}
		return $strArchiveFileContents;
	}



