<?php

	require_once("wdk_error.inc");

	interface ICache
	{
		function GetCacheObject($strID);
		function SetCacheObject($strID,$vCacheObject,$nSecondsTTL);
		function DeleteCacheObject($strID);
		function ClearCache();
		function FlushCache();
		function GetAllCacheObjectInfos();
	}
	
	
	class CMemoryCache implements ICache
	{
		private $m_arrayCacheObjects;
	
		function __construct()
		{
			$this->m_arrayCacheObjects = array();
		}
		
		function GetCacheObject($strID)
		{
			$timeExpiry = ArrayGetValue($this->m_arrayCacheObjects,$strID,'expiry');

			if ($timeExpiry == false)
			{
				return false;	
			}
			if (GetTimeNow() >= $timeExpiry)
			{
				$this->DeleteCacheObject($strID);
				return false;	
			}
			return ArrayGetValue($this->m_arrayCacheObjects,$strID,'payload');
		}
		
		function SetCacheObject($strID,$vCacheObject,$nSecondsTTL)
		{
			$this->m_arrayCacheObjects[$strID]['expiry'] = GetTimeAddSeconds(GetTimeNow(),$nSecondsTTL);
			$this->m_arrayCacheObjects[$strID]['payload'] = $vCacheObject;
			return true;
		}
		
		function DeleteCacheObject($strID)
		{
			$this->m_arrayCacheObjects = ArrayRemoveKey($this->m_arrayCacheObjects,$strID);
			return true;
		}
		
		function ClearCache()
		{
			foreach ($this->m_arrayCacheObjects as $strID => $arrayCacheObject)
			{
				$timeExpiry = ArrayGetValue($arrayCacheObject,'expiry');
				if (GetTimeNow() >= $timeExpiry)
				{
					$this->DeleteCacheObject($strID);
				}
			}
			return true;
		}

		function FlushCache()
		{
			$this->m_arrayCacheObjects = array();
			return true;
		}
		
		function GetAllCacheObjectInfos()
		{
			$arrayCacheObjectInfos = array();
			foreach ($this->m_arrayCacheObjects as $strID => $arrayCacheObject)
			{
				if ($strID != '')
				{
					$timeExpiry = ArrayGetValue($arrayCacheObject,'expiry');
					$strExpiry = RenderDateTime($timeExpiry);
					$arrayCacheObjectInfos[$strID]['id'] = $strID;
					$arrayCacheObjectInfos[$strID]['expiry'] = $strExpiry;
				}
			}
			return $arrayCacheObjectInfos;
		}

	}
	
	
	class CFileCache implements ICache
	{
		private $m_strCacheDir;
		private $m_strScopeID;
		
		function __construct($strCacheDir,$strScopeID = 'default')
		{
			$this->m_strCacheDir = null;
			$this->SetCacheDirectory($strCacheDir);
			$this->m_strScopeID = $strScopeID;
		}

		function SetCacheDirectory($strCacheDir)
		{
			if (IsDirectoryReadWriteAccess($strCacheDir))
			{
				$this->m_strCacheDir = $strCacheDir;
				return true;
			}
			$this->m_strCacheDir = null;
			return false;
		}

		function GetCacheDirectory()
		{
			return $this->m_strCacheDir;
		}
		
		function GetScopeID()
		{
			return $this->m_strScopeID;	
		}
		
		function IsReady()
		{
			return IsDirectoryReadWriteAccess($this->GetCacheDirectory());
		}		
		
		function MakeCacheObjectFileName($strID, $nSecondsTTL)
		{
			$strDateTime = RenderDateTime(GetTimeAddSeconds(GetTimeNow(),$nSecondsTTL));
			$strDateTime = ReplaceString($strDateTime,':','-');
			$strDateTime = ReplaceString($strDateTime,' ','-');
			return
				'cache_'.
				$this->GetScopeID().'_'.
				$strDateTime.'_'.
				$strID.
				'.cache';
		}
		
		function IsValidID($strID)
		{
			if (StringLength($strID) < 1)
			{
				return false;	
			}
			if (!StringCheckCharSet($strID,'-'.CHARSET_ALPHANUMERIC_LOWERCASE))
			{
				return false;	
			}
			return true;
		}
		
		function ParseCacheObjectFileName($strFileName)
		{
			//cache_s_2020-04-27-10-15-11_a.cache		
		
			if (StringLength($strFileName) < (5 +1+ 1 +1+ 19 +1+ 1 +1+ 5))
			{
				return false;
			}
			if (StringSection($strFileName,0,6) != 'cache_')
			{
				return false;	
			}
			if (StringSection($strFileName,-6) != '.cache')
			{
				return false;	
			}

			$strRemaining = StringSection($strFileName,6);

			$nPosScopeIdEnd = FindString($strRemaining,'_');
			if ($nPosScopeIdEnd == -1)
			{
				return false;	
			}
			$strScopeID = StringSection($strRemaining,0,$nPosScopeIdEnd);

			$strRemaining = StringSection($strRemaining,$nPosScopeIdEnd+1);
			
			$strDateTime = StringSection($strRemaining,0,19);  
			$strDateTime = 
				StringSection($strDateTime,0,10).
				' '.
				StringSection($strDateTime,11,2).
				':'.
				StringSection($strDateTime,14,2).
				':'.
				StringSection($strDateTime,17,2); 
			
			$strRemaining = StringSection($strRemaining,19+1);
			
			$nPosIdEnd = FindString($strRemaining,'.cache');
			if ($nPosIdEnd == -1)
			{
				return false;	
			}
			$strID = StringSection($strRemaining,0,$nPosIdEnd);
			
			$arrayFileInfo = array();
			$arrayFileInfo['scope'] = $strScopeID;
			$arrayFileInfo['expiry'] = $strDateTime;
			$arrayFileInfo['id'] = $strID;
			return $arrayFileInfo;
		}
		
		function GetCacheObject($strID)
		{
			if (!$this->IsValidID($strID))
			{
				return false;	
			}
			$arrayFiles = ReadDirectory($this->GetCacheDirectory());
			if ($arrayFiles == false)
			{
				return false;	
			}
			$arrayFiles = ArraySort($arrayFiles);
			foreach ($arrayFiles as $strFileName)
			{
				$arrayFileInfo = $this->ParseCacheObjectFileName($strFileName);
				if (IsArray($arrayFileInfo))
				{
					if ($this->GetScopeID() == ArrayGetValue($arrayFileInfo,'scope'))
					{
						if ($strID == ArrayGetValue($arrayFileInfo,'id'))
						{
							$strDateTime = ArrayGetValue($arrayFileInfo,'expiry'); 
							$timeExpiry = GetTime($strDateTime);
							if (GetTimeNow() >= $timeExpiry)
							{
								$this->DeleteCacheObject($strID);
							}
							else
							{
								$strFilePath = $this->GetCacheDirectory() . $strFileName;
								$vSerializedCacheObject = FileRead($strFilePath);
								return unserialize($vSerializedCacheObject);
							}
						}
					}
				}
			}
			return false;
		}
		
		function SetCacheObject($strID,$vCacheObject,$nSecondsTTL)
		{
			if (!$this->IsValidID($strID))
			{
				return false;	
			}
			$this->DeleteCacheObject($strID);
			$strFileName = $this->MakeCacheObjectFileName($strID,$nSecondsTTL);
			$strFilePath = $this->GetCacheDirectory().$strFileName;
			$vSerializedCacheObject = serialize($vCacheObject);
			return FileWrite($strFilePath,$vSerializedCacheObject,true);
		}
		
		function DeleteCacheObject($strID)
		{
			if (!$this->IsValidID($strID))
			{
				return false;	
			}
			$arrayFiles = ReadDirectory($this->GetCacheDirectory());
			if ($arrayFiles == false)
			{
				return false;	
			}
			foreach ($arrayFiles as $strFileName)
			{
				$arrayFileInfo = $this->ParseCacheObjectFileName($strFileName);
				if (IsArray($arrayFileInfo))
				{
					if ($this->GetScopeID() == ArrayGetValue($arrayFileInfo,'scope'))
					{
						if ($strID == ArrayGetValue($arrayFileInfo,'id'))
						{
							$strFilePath = $this->GetCacheDirectory() . $strFileName;
							DeleteFile($strFilePath); 
						}
					}
				}
			}
			return true;			
		}
		
		function ClearCache()
		{
			$arrayFiles = ReadDirectory($this->GetCacheDirectory());
			if ($arrayFiles == false)
			{
				return false;	
			}
			foreach ($arrayFiles as $strFileName)
			{
				$arrayFileInfo = $this->ParseCacheObjectFileName($strFileName);
				if (IsArray($arrayFileInfo))
				{
					if ($this->GetScopeID() == ArrayGetValue($arrayFileInfo,'scope'))
					{
						$strDateTime = ArrayGetValue($arrayFileInfo,'expiry');
						$timeExpiry = GetTime($strDateTime);
						if (GetTimeNow() >= $timeExpiry)
						{
							$strFilePath = $this->GetCacheDirectory() . $strFileName;
							DeleteFile($strFilePath); 
						}
					}
				}
			}
		}

		function FlushCache()
		{
			$arrayFiles = ReadDirectory($this->GetCacheDirectory());
			if ($arrayFiles == false)
			{
				return false;	
			}
			foreach ($arrayFiles as $strFileName)
			{
				$arrayFileInfo = $this->ParseCacheObjectFileName($strFileName);
				if (IsArray($arrayFileInfo))
				{
					if ($this->GetScopeID() == ArrayGetValue($arrayFileInfo,'scope'))
					{
						$strFilePath = $this->GetCacheDirectory() . $strFileName;
						DeleteFile($strFilePath); 
					}
				}
			}
		}
		
		function GetAllCacheObjectInfos()
		{
			$arrayCacheObjectInfos = array();	
			$arrayFiles = ReadDirectory($this->GetCacheDirectory());
			if ($arrayFiles == false)
			{
				return false;	
			}
			foreach ($arrayFiles as $strFileName)
			{
				$arrayFileInfo = $this->ParseCacheObjectFileName($strFileName);
				if (IsArray($arrayFileInfo))
				{
					if ($this->GetScopeID() == ArrayGetValue($arrayFileInfo,'scope'))
					{
						$strID = ArrayGetValue($arrayFileInfo,'id');
						$strExpiry = ArrayGetValue($arrayFileInfo,'expiry');
						$arrayCacheObjectInfos[$strID]['id'] = $strID;
						$arrayCacheObjectInfos[$strID]['expiry'] = $strExpiry;
					}
				}
			}
			return $arrayCacheObjectInfos;
		}
	}
