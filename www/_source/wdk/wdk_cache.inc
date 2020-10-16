<?php

	require_once("wdk_error.inc");

	interface ICache
	{
		function GetCacheObject($strID);
		function SetCacheObject($strID,$vCacheObject,$nSecondsTTL);
		function DeleteCacheObject($strID);
		function ClearCache();
		function FlushCache();
	}
	
	
	class CInstanceMemoryCache implements ICache
	{
		private $m_arrayCacheObjects;
	
		function _construct()
		{
			$this->m_arrayCacheObjects = array();
		}
		
		function GetCacheObject($strID)
		{
			$timeExpiry = ArrayGetValue($this->m_arrayCacheObjects,$strID,"expiry");

			if ($timeExpiry == false)
			{
				return false;	
			}
			if (GetTimeNow() > $timeExpiry)
			{
				$this->DeleteCacheObject($strID);
				return false;	
			}
			return ArrayGetValue($this->m_arrayCacheObjects,$strID,"payload");
		}
		
		function SetCacheObject($strID,$vCacheObject,$nSecondsTTL)
		{
			$this->m_arrayCacheObjects[$strID]["expiry"] = GetTimeAddSeconds(GetTimeNow(),$nSecondsTTL);
			$this->m_arrayCacheObjects[$strID]["payload"] = $vCacheObject;
		}
		
		function DeleteCacheObject($strID)
		{
			$this->m_arrayCacheObjects = ArrayRemoveKey($this->m_arrayCacheObjects,$strID);
		}
		
		function ClearCache()
		{
			foreach ($this->m_arrayCacheObjects as $arrayCacheObject)
			{
				$timeExpiry = ArrayGetValue($arrayCacheObject,"expiry");
				if (GetTimeNow() > $timeExpiry)
				{
					$this->DeleteCacheObject($strID);
					return false;	
				}
			}
		}

		function FlushCache()
		{
			$this->m_arrayCacheObjects = array();
		}

	}
	
	/*
	
	class CFileCache implements ICache
	{
		private $m_strCacheDir;
		
		function _construct($strCacheDir)
		{
			$this->m_strCacheDir = "";
			//if (IsDirectory($strCacheDir))
			
			$this->m_strCacheDir = $strCacheDir;
			
		}
		
		function GetCacheObject($strID)
		{
			
		}
		
		function SetCacheObject($strID,$vCacheObject)
		{
			
		}
		
		function DeleteCacheObject($strID)
		{
			
		}
		
		function ClearCache()
		{
			// Delete all files beyond their ttl
			
		}

		function FlushCache()
		{
			// Delete all files in cache dir
			
		}

	}
	*/
	



