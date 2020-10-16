<?php

	abstract class CSingleton
	{
		static private $m_instance = NULL;
		
		private
		function __construct(){}
		
		private
		function __clone(){}		
		
		static public
		function GetInstance()
		{
			if (self::$m_instance === NULL)
			{
				self::$m_instance = new self;
			}
			return self::$m_instance;
		}
	}


	class CGenericSingleton
	{
		static private $m_arrayInstances = array();
		
		static public
		function GetInstance($strClassName)
		{
			if (!isset(self::$m_arrayInstances[$strClassName]))
			{
				self::$m_arrayInstances[$strClassName] = new $strClassName();
			}
			return self::$m_arrayInstances[$strClassName];
		}
	}

