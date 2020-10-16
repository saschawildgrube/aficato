<?php

	class CStopWatch
	{
		private $m_timeStart;
		private $m_timeStop;
		private $m_fTotalSeconds;
		
		function __construct()
		{
			$this->Reset();
		}
		
		function Reset()
		{
			$this->m_timeStart = 0;
			$this->m_timeStop = 0;
			$this->m_fTotalSeconds = 0;
		}
		
		function Start()
		{
			$this->m_timeStart = GetTimeNow();
			$this->m_timeStop = 0;
		}
		
		function Stop()
		{
			$this->m_timeStop = GetTimeNow();
			$this->m_fTotalSeconds += $this->GetSeconds();
		}
		
		function GetSeconds()
		{
			if ($this->m_timeStart == 0)
			{
				return 0;
			}
			if ($this->m_timeStop == 0)
			{
				return GetTimeNow() - $this->m_timeStart;
			}
			else
			{
				return $this->m_timeStop - $this->m_timeStart;
			}
		}
		
		function GetTotalSeconds()
		{
			if ($this->m_timeStart == 0)
			{
				return 0;
			}
			if ($this->m_timeStop == 0)
			{
				return GetTimeNow() - $this->m_timeStart + $this->m_fTotalSeconds;
			}
			else
			{
				return $this->m_fTotalSeconds;
			}
		}
		
		function GetStart()
		{
			return $this->m_timeStart;
		}
		
		function GetStop()
		{
			return $this->m_timeStop;
		}
	}



