all: birdSearch.c cleanup.c deleteObs.c list_of_birds_seen_in_a_given_year.c menuSystem.c sightings_stats_mthwise.c sightings_stats_yrwise.c
	gcc birdSearch.c -o birdSearch.out
	gcc cleanup.c -o cleanup.out
	gcc deleteObs.c -o deleteObs.out
	gcc list_of_birds_seen_in_a_given_year.c -o list_of_birds_seen_in_a_given_year.out
	gcc menuSystem.c -o menuSystem.out
	gcc sightings_stats_mthwise.c -o sightings_stats_mthwise.out
	gcc sightings_stats_yrwise.c -o sightings_stats_yrwise.out