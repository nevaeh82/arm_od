ALTER TABLE `Info`
	ADD COLUMN `latitudeStdDev` DOUBLE NULL AFTER `altitude`,
	ADD COLUMN `longitudeStdDev` DOUBLE NULL AFTER `latitudeStdDev`,
	ADD COLUMN `altidudeStdDev` DOUBLE NULL DEFAULT NULL AFTER `longitudeStdDev`;