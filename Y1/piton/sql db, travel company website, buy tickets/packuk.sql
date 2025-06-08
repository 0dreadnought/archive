-- phpMyAdmin SQL Dump
-- version 5.2.0
-- https://www.phpmyadmin.net/
--
-- Хост: 127.0.0.1
-- Время создания: Апр 09 2023 г., 16:39
-- Версия сервера: 10.4.27-MariaDB
-- Версия PHP: 8.2.0

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- База данных: `packuk`
--

-- --------------------------------------------------------

--
-- Структура таблицы `bookings`
--

CREATE TABLE `bookings` (
  `sname` varchar(40) NOT NULL,
  `depar` varchar(40) NOT NULL,
  `dest` varchar(40) NOT NULL,
  `type` varchar(40) NOT NULL,
  `date` varchar(40) NOT NULL,
  `refernumber` varchar(40) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Дамп данных таблицы `bookings`
--

INSERT INTO `bookings` (`sname`, `depar`, `dest`, `type`, `date`, `refernumber`) VALUES
('hach', 'Manchester', 'Birmingham', 'Train', '2004-12-13', '1680999802503973'),
('None', 'Manchester', 'Birmingham', 'Train', '2004-12-13', '1680999802349496'),
('hach', 'Birmingham', 'Manchester', 'Train', '2004-12-15', '1680999802931801'),
('None', 'Birmingham', 'Manchester', 'Train', '2004-12-15', '1680999802212882'),
('hach', 'Manchester', 'Birmingham', 'Plane', '2004-12-13', '16810463333333622373'),
('hach', 'Birmingham', 'Manchester', 'None', 'None', '16810463339587056442'),
('hach', 'Manchester', 'Birmingham', 'Plane', '2004-12-13', '16810463587488243055'),
('Artem', 'Manchester', 'Birmingham', 'Train', '2004-12-13', '16810464921560492284');

-- --------------------------------------------------------

--
-- Структура таблицы `trips`
--

CREATE TABLE `trips` (
  `Depar` text NOT NULL,
  `Dest` text NOT NULL,
  `DoW` int(11) NOT NULL,
  `Price` int(11) NOT NULL,
  `JourT` int(11) NOT NULL,
  `type` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Дамп данных таблицы `trips`
--

INSERT INTO `trips` (`Depar`, `Dest`, `DoW`, `Price`, `JourT`, `type`) VALUES
('Manchester', 'London', 0, 10, 4, 1),
('Manchester', 'Birmingham', 0, 6, 5, 1),
('Edinburgh', 'Manchester', 0, 8, 4, 1),
('Edinburgh', 'London', 1, 12, 3, 1),
('London', 'Newcastle', 1, 6, 4, 1),
('London', 'Birmingham', 1, 10, 6, 1),
('Birmingham', 'London', 2, 8, 3, 1),
('Birmingham', 'Manchester', 2, 10, 4, 1),
('Bristol', 'Glasgow', 3, 12, 5, 1),
('Bristol', 'Cardiff', 3, 14, 3, 1),
('Newcastle', 'Cardiff', 4, 12, 4, 1),
('Newcastle', 'London', 4, 12, 5, 1),
('Cardiff', 'Birmingham', 4, 10, 6, 1),
('Cardiff', 'Glasgow', 5, 12, 4, 1),
('Glasgow', 'Manchester', 5, 18, 4, 1),
('Glasgow', 'London', 6, 22, 3, 1),
('Manchester', 'London', 0, 16, 2, 2),
('Manchester', 'Birmingham', 0, 18, 3, 2),
('Edinburgh', 'Glasgow', 1, 16, 4, 2),
('Edinburgh', 'Manchester', 1, 16, 3, 2),
('London', 'Birmingham', 2, 30, 3, 2),
('London', 'Newcastle', 2, 32, 3, 2),
('Birmingham', 'Manchester', 2, 18, 3, 2),
('Birmingham', 'London', 2, 18, 4, 2),
('Bristol', 'London', 2, 22, 3, 2),
('Bristol', 'Manchester', 2, 20, 3, 2),
('Newcastle', 'Glasgow', 3, 44, 3, 2),
('Newcastle', 'Cardiff', 4, 34, 3, 2),
('Cardiff', 'London', 4, 36, 4, 2),
('Cardiff', 'Newcastle', 5, 36, 4, 2),
('Glasgow', 'Newcastle', 5, 50, 4, 2),
('Glasgow', 'Birmingham', 6, 58, 3, 2),
('Manchester', 'London', 0, 40, 1, 3),
('Manchester', 'Birmingham', 0, 25, 1, 3),
('Edinburgh', 'Manchester', 1, 50, 2, 3),
('London', 'Birmingham', 2, 30, 1, 3),
('Birmingham', 'Manchester', 2, 20, 1, 3),
('Birmingham', 'London', 2, 30, 1, 3),
('Bristol', 'London', 2, 20, 1, 3),
('Bristol', 'Manchester', 2, 25, 1, 3),
('Glasgow', 'Birmingham', 4, 60, 2, 3);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
