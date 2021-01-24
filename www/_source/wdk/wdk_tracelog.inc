<?php

	class CTraceLog
	{
		private $m_strTraceLog;
		private $m_strPrefix;
		private $m_strPostfix;
		private $m_nDebugPoint;

		function __construct()
		{
			$this->m_strPrefix = '';
			$this->m_strPostfix = "\n";
			$this->m_nDebugPoint = 0;
		}
		
		function SetPrefix($strNewPrefix)
		{
			$this->m_strPrefix = $strNewPrefix;	
		}
		
		function SetPostfix($strNewPostfix)
		{
			$this->m_strPostfix = $strNewPostfix;	
		}
	
		function Trace($value)
		{
			if (is_string($value))
			{
				$this->m_strTraceLog .=	$this->m_strPrefix.$value.$this->m_strPostfix;
			}
			else
			{
				$this->m_strTraceLog .= $this->m_strPrefix.RenderValue($value).$this->m_strPostfix;
			}
		}
		
		function GetTraceLog()
		{
			return $this->m_strTraceLog;
		}
		
		function RenderTraceLog()
		{
			return "<div style=\"float:left;\"><pre style=\"text-align: left;\">" . HtmlEncode($this->GetTraceLog()) . "</pre></div>";	
		}
		
		function Debug($value = null)
		{
			if ($this->m_nDebugPoint == 0)
			{
				$this->m_strTraceLog = $this->m_strPrefix.'Debug mode ACTIVE'.$this->m_strPostfix.$this->m_strTraceLog;
			}
			$this->m_nDebugPoint++;
			$this->Trace("DEBUG: #".$this->m_nDebugPoint.''.(($value !== null && !is_array($value))?(': '.RenderValue($value)):('')));
			if (is_array($value) == true)
			{
				$this->Trace($value);	
			}
		}
		
		function IsDebug()
		{
			return ($this->m_nDebugPoint > 0)?(true):(false);
		}

	}

