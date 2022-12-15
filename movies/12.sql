SELECT title FROM movies, people, stars
WHERE stars.person_id = people.id
AND stars.movie_id = movies.id
AND (name = "Johnny Depp" OR name = "Helena Bonham Carter")
GROUP BY title
HAVING COUNT(name) == 2;