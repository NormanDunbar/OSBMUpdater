==================================
Deployment process for Norm to Do!
==================================

PreProd (ppdcfg)
----------------


#.	Take incremental backup/Create restore point
#.	Give required users restricted session on the desired database
#.	Put the desired database in restricted mode
#.	The scripts are found on ``$/TA/MAIN/Non Source/Dev DBA/Database Release/Active/R804 May Release/R804_V01_P00_MayRel_db_release_master.zip`` - but this will vary, see "go-ahead" email for full details.
#.	Copy the file on the server in ``F:\_devops\Releases\YYYYMon``
#.	After unzipping the file run ``sqlplus /nolog@R804_V01_P00_MayRel_db_release_master.sql`` on the desired database.
#.	Check the logs
#.	Disable restricted session
#.	Do a schema compare between PRDUAT and PreProd
#.	Send results to Lead dev

Prod (cfg)
----------
The steps are identiocal to steps 1-6 above.

7.  Run IO Terminal Adhoc Fix attached
#.  Check USER folder as extracted from zip. 
#.  For each user, generate a secure password and amend the script.
#.  Note the username/password combinations.
#.	Check the logs
#.	Disable restricted session
#.	Do a schema compare between PreProd and Live.
#.	Send results to Lead dev.
#.  Send username/password combinations to Windows (DevOps) support on the night - Yussuf, Andy W, etc.
