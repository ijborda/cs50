SELECT title FROM movies, stars, ratings, people
WHERE stars.movie_id = movies.id
AND ratings.movie_id = movies.id
AND people.id = stars.person_id
AND name = "Chadwick Boseman"
ORDER by rating DESC
LIMIT 5;
