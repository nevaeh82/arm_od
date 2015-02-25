ALTER IGNORE TABLE `Info`
	ADD UNIQUE INDEX `uavID_device_source_datetime` (`uavID`, `device`, `source`, `datetime`),
	ORDER BY id DESC;