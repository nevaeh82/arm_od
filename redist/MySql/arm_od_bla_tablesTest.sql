SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

CREATE SCHEMA IF NOT EXISTS `BLATEST` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;
USE `BLATEST` ;

-- -----------------------------------------------------
-- Table `BLATEST`.`TypeBLADictionary`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BLATEST`.`TypeBLADictionary` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` TEXT NULL,
  PRIMARY KEY (`id`),
  INDEX `typeID` (`id` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `BLATEST`.`BLA`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BLATEST`.`BLA` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `idBLA` INT UNSIGNED NOT NULL,
  `ip` TEXT NOT NULL,
  `type` INT NULL,
  `name` TEXT NULL,
  PRIMARY KEY (`id`),
  INDEX `typeID` (`type` ASC),
  INDEX `id` (`id` ASC),
  CONSTRAINT `typeID`
    FOREIGN KEY (`type`)
    REFERENCES `BLATEST`.`TypeBLADictionary` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `BLATEST`.`DeviceDictionary`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BLATEST`.`DeviceDictionary` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(45) NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `BLATEST`.`Devices`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BLATEST`.`Devices` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `deviceID` INT NULL,
  `port` MEDIUMINT NULL,
  `BLAID` INT NULL,
  PRIMARY KEY (`id`),
  INDEX `BLAID` (`BLAID` ASC),
  INDEX `deviceID` (`deviceID` ASC),
  CONSTRAINT `device`
    FOREIGN KEY (`deviceID`)
    REFERENCES `BLATEST`.`DeviceDictionary` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `BLATEST`.`StatusDictionary`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BLATEST`.`StatusDictionary` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` TEXT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `BLATEST`.`Info`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BLATEST`.`Info` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `BLA_id` INT NULL,
  `device` INT NULL,
  `latitude` DOUBLE NOT NULL,
  `longitude` DOUBLE NOT NULL,
  `altitude` DOUBLE NULL,
  `speed` DOUBLE NULL,
  `yaw` DOUBLE NULL,
  `restTime` TIME NULL,
  `statusID` INT NULL,
  `datetime` DATETIME NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `IBLAID` (`BLA_id` ASC),
  INDEX `deviceID` (`device` ASC),
  INDEX `st` (`statusID` ASC),
  CONSTRAINT `deviceID`
    FOREIGN KEY (`device`)
    REFERENCES `BLATEST`.`Devices` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `IBLAID`
    FOREIGN KEY (`BLA_id`)
    REFERENCES `BLATEST`.`BLA` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `stID`
    FOREIGN KEY (`statusID`)
    REFERENCES `BLATEST`.`StatusDictionary` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `BLATEST`.`TargetTypeDictionary`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BLATEST`.`TargetTypeDictionary` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` TEXT NULL,
  PRIMARY KEY (`id`),
  INDEX `id` (`id` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `BLATEST`.`Target`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BLATEST`.`Target` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `ip` TEXT NULL,
  `port` MEDIUMINT NULL,
  `targetID` INT NULL,
  `type` INT NULL,
  PRIMARY KEY (`id`),
  INDEX `TtypeID` (`type` ASC),
  CONSTRAINT `TtypeID`
    FOREIGN KEY (`type`)
    REFERENCES `BLATEST`.`TargetTypeDictionary` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `BLATEST`.`BLAMission`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BLATEST`.`BLAMission` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `BLAid` INT NULL,
  `targetID` INT NULL,
  `regionCenterLatitude` DOUBLE NULL,
  `regionCenterLongitude` DOUBLE NULL,
  `regionCenterAtitude` DOUBLE NULL,
  `regionRadius` DOUBLE NULL,
  `timeToTarget` TIME NULL,
  PRIMARY KEY (`id`),
  INDEX `BLAid` (`BLAid` ASC),
  INDEX `targetID` (`targetID` ASC),
  CONSTRAINT `BLAID`
    FOREIGN KEY (`BLAid`)
    REFERENCES `BLATEST`.`BLA` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `targetID`
    FOREIGN KEY (`targetID`)
    REFERENCES `BLATEST`.`Target` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
