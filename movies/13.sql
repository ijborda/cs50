SELECT DISTINCT(name) FROM movies, stars, people
WHERE movies.id = stars.movie_id
AND stars.person_id = people.id
AND movie_id IN
    (SELECT movie_id FROM movies, stars, people
    WHERE movies.id = stars.movie_id
    AND stars.person_id = people.id
    AND name = "Kevin Bacon"
    AND birth = 1958)
AND name != "Kevin Bacon";