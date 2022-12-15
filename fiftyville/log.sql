-- Keep a log of any SQL queries you execute as you solve the mystery.


-- Check crime scene reports which took place on July 28, 2021 at Humphrey Street.
SELECT * FROM crime_scene_reports
WHERE street = "Humphrey Street"
AND year = 2021
AND month = 7
AND day = 28;
-- There are two results: id 295 and id 297.
-- Case id 297 is about littering, so this can be disregarded.
-- Case id 295 is about the theft of the CS50 duck. Time of crime is on 10:15 am.
-- There are 3 interviews, all their transcipts mention bakery.


-- Check transcript of interviews. Use bakery as a keyword.
SELECT * FROM interviews
WHERE year = 2021
AND month = 7
AND day = 28
AND transcript LIKE "%bakery%";
-- Three witnesses were found: Ruth, Eugene, and Raymond. Emma, the owner of the bakery, also also seems to saw a suspicious person.
-- Information from Ruth: See security footage and look for cars that left the bakery parking lot at the same time frame.
-- Information from Eugene: See records of withdrawal in the ATM on Leggett Street. Thief was someone Eugene recognized.
-- Information from Raymond: Thief called someone to buy the earliest flight ticket out of Fiftyville tomorrow. The call lasted less than a minute.
-- Information from Emma: Someone suspicious was on their phone for about half an hour.


-- See atm withdrawal at Leggett Street on the same day. Get names of persons with matching transactions.
SELECT * FROM people
WHERE id IN
    (SELECT person_id FROM bank_accounts
    WHERE account_number IN
        (SELECT account_number FROM atm_transactions
        WHERE year = 2021
        AND month = 7
        AND day = 28
        AND atm_location = "Leggett Street"
        AND transaction_type = "withdraw"));
-- There are 8 suspects: Kenny ($48), Iman($20), Benista($60), Taylor(5$0), Brooke($80), Luca($20), Diana($30), and Bruce($35)
-- Profile of the suspects:
-- +--------+---------+----------------+-----------------+---------------+
-- |   id   |  name   |  phone_number  | passport_number | license_plate |
-- +--------+---------+----------------+-----------------+---------------+
-- | 395717 | Kenny   | (826) 555-1652 | 9878712108      | 30G67EN       |
-- | 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
-- | 438727 | Benista | (338) 555-6650 | 9586786673      | 8X428L0       |
-- | 449774 | Taylor  | (286) 555-6063 | 1988161715      | 1106N58       |
-- | 458378 | Brooke  | (122) 555-4581 | 4408372428      | QX4YZN3       |
-- | 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +--------+---------+----------------+-----------------+---------------+


-- Find the earliest flight ticket out of Fiftyville tomorrow
SELECT * FROM airports
WHERE id =
   (SELECT destination_airport_id FROM flights
    WHERE origin_airport_id =
        (SELECT id FROM airports
        WHERE city = "Fiftyville")
    AND year = 2021
    AND month = 7
    AND day = 29
    ORDER BY hour ASC, minute ASC
    LIMIT 1);
-- Earliest flight goes to New York City via the LaGuardia Airport
-- See passenger list of this flight
SELECT * FROM passengers
WHERE flight_id =
    (SELECT id FROM flights
    WHERE origin_airport_id =
            (SELECT id FROM airports
            WHERE city = "Fiftyville")
        AND year = 2021
        AND month = 7
        AND day = 29
        ORDER BY hour ASC, minute ASC
        LIMIT 1);
-- Result:
-- +-----------+-----------------+------+
--| flight_id | passport_number | seat |
--+-----------+-----------------+------+
--| 36        | 7214083635      | 2A   |
--| 36        | 1695452385      | 3B   |
--| 36        | 5773159633      | 4A   |
--| 36        | 1540955065      | 5C   |
--| 36        | 8294398571      | 6C   |
--| 36        | 1988161715      | 6D   |
--| 36        | 9878712108      | 7A   |
--| 36        | 8496433585      | 7B   |
--+-----------+-----------------+------+


-- Cross-reference the passengers in the flight and those who withdraw in the ATM using the passport number
SELECT * FROM passengers
WHERE flight_id =
    (SELECT id FROM flights
    WHERE origin_airport_id =
            (SELECT id FROM airports
            WHERE city = "Fiftyville")
        AND year = 2021
        AND month = 7
        AND day = 29
        ORDER BY hour ASC, minute ASC
        LIMIT 1)
AND passport_number IN
    (SELECT passport_number FROM people
    WHERE id IN
        (SELECT person_id FROM bank_accounts
        WHERE account_number IN
            (SELECT account_number FROM atm_transactions
            WHERE year = 2021
            AND month = 7
            AND day = 28
            AND atm_location = "Leggett Street"
            AND transaction_type = "withdraw")));
-- Suspects now narrowed down to four people:
-- +-----------+-----------------+------+
-- | flight_id | passport_number | seat |
-- +-----------+-----------------+------+
-- | 36        | 5773159633      | 4A   |   Bruce
-- | 36        | 1988161715      | 6D   |   Taylor
-- | 36        | 9878712108      | 7A   |   Kenny
-- | 36        | 8496433585      | 7B   |   Luca
-- +-----------+-----------------+------+


-- See phone calls that are less than a minute
SELECT * FROM phone_calls
WHERE year = 2021
AND month = 7
AND day = 28
AND duration < 60;
-- Results
-- +-----+----------------+----------------+------+-------+-----+----------+
--| id  |     caller     |    receiver    | year | month | day | duration |
--+-----+----------------+----------------+------+-------+-----+----------+
--| 221 | (130) 555-0289 | (996) 555-8899 | 2021 | 7     | 28  | 51       |
--| 224 | (499) 555-9472 | (892) 555-8872 | 2021 | 7     | 28  | 36       |
--| 233 | (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45       |
--| 251 | (499) 555-9472 | (717) 555-1342 | 2021 | 7     | 28  | 50       |
--| 254 | (286) 555-6063 | (676) 555-6554 | 2021 | 7     | 28  | 43       |
--| 255 | (770) 555-1861 | (725) 555-3243 | 2021 | 7     | 28  | 49       |
--| 261 | (031) 555-6622 | (910) 555-3251 | 2021 | 7     | 28  | 38       |
--| 279 | (826) 555-1652 | (066) 555-9701 | 2021 | 7     | 28  | 55       |
--| 281 | (338) 555-6650 | (704) 555-2131 | 2021 | 7     | 28  | 54       |
--+-----+----------------+----------------+------+-------+-----+----------+


-- Crossreference four suspects to suspected call logs
SELECT * FROM phone_calls
WHERE year = 2021
AND month = 7
AND day = 28
AND duration < 60
AND caller IN
    (SELECT phone_number FROM people
    WHERE passport_number IN
        (SELECT passport_number FROM passengers
        WHERE flight_id =
            (SELECT id FROM flights
            WHERE origin_airport_id =
                    (SELECT id FROM airports
                    WHERE city = "Fiftyville")
                AND year = 2021
                AND month = 7
                AND day = 29
                ORDER BY hour ASC, minute ASC
                LIMIT 1)
        AND passport_number IN
            (SELECT passport_number FROM people
            WHERE id IN
                (SELECT person_id FROM bank_accounts
                WHERE account_number IN
                    (SELECT account_number FROM atm_transactions
                    WHERE year = 2021
                    AND month = 7
                    AND day = 28
                    AND atm_location = "Leggett Street"
                    AND transaction_type = "withdraw")))));
-- Suspect reduced to three persons
-- Results:
--+-----+----------------+----------------+------+-------+-----+----------+
--| id  |     caller     |    receiver    | year | month | day | duration |
--+-----+----------------+----------------+------+-------+-----+----------+
--| 233 | (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45       | Bruce
--| 254 | (286) 555-6063 | (676) 555-6554 | 2021 | 7     | 28  | 43       | Taylor
--| 279 | (826) 555-1652 | (066) 555-9701 | 2021 | 7     | 28  | 55       | Kenny
--+-----+----------------+----------------+------+-------+-----+----------+


-- See security footage from Bakery
SELECT license_plate FROM bakery_security_logs
WHERE year = 2021
AND month = 7
AND day = 28
AND activity = "exit"
AND hour = 10
AND minute >= 15
AND minute <= 25)
-- Cross reference it to the three suspected persons
SELECT * FROM people
WHERE phone_number IN
    (SELECT caller FROM phone_calls
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND duration < 60
    AND caller IN
        (SELECT phone_number FROM people
        WHERE passport_number IN
            (SELECT passport_number FROM passengers
            WHERE flight_id =
                (SELECT id FROM flights
                WHERE origin_airport_id =
                        (SELECT id FROM airports
                        WHERE city = "Fiftyville")
                    AND year = 2021
                    AND month = 7
                    AND day = 29
                    ORDER BY hour ASC, minute ASC
                    LIMIT 1)
            AND passport_number IN
                (SELECT passport_number FROM people
                WHERE id IN
                    (SELECT person_id FROM bank_accounts
                    WHERE account_number IN
                        (SELECT account_number FROM atm_transactions
                        WHERE year = 2021
                        AND month = 7
                        AND day = 28
                        AND atm_location = "Leggett Street"
                        AND transaction_type = "withdraw"))))))
AND license_plate IN
    (SELECT license_plate FROM bakery_security_logs
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND activity = "exit"
    AND hour = 10
    AND minute >= 15
    AND minute <= 25);
-- Results reduced to two suspects
--+--------+-------+----------------+-----------------+---------------+
--|   id   | name  |  phone_number  | passport_number | license_plate |
--+--------+-------+----------------+-----------------+---------------+
--| 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
--+--------+-------+----------------+-----------------+---------------+
-- Bruce is the suspect.


-- Find the accomplice
SELECT name FROM people
WHERE phone_number = "(375) 555-8161";
-- Accomplice is Robin