# Simulate some 6 card poker hands
cat('Problem 1\n')
ntrials = 100
size_hand = 6
nranks = 13
nsuits = 4
deck = c(1:(nranks*nsuits))
is_two_pair = function(hand) {
  hand = hand %% nranks # remove suit information
  ranks_in_hand = hand[!duplicated(hand)]
  ret = FALSE
  npair = 0
  for (r in ranks_in_hand) {
    u = sum(hand==r)
    if (u == 2) {
      npair = npair + 1
    }
    else if (u > 2) { # 3 or more of a kind
      npair = 0
      break
    }
  }
  if (npair == 2) {
    ret = TRUE
  }
  return(ret)
}
is_three_of_a_kind = function(hand) {
  hand = hand %% nranks # remove suit information
  ranks_in_hand = hand[!duplicated(hand)]
  ret = FALSE
  ntriples = 0
  for (r in ranks_in_hand) {
    u = sum(hand==r)
    if (u == 3) {
      ntriples = ntriples + 1
    }
    else if (u == 2 || u > 3) { # Found pair or more than 3 of a kind
      ntriples = 0
      break
    }
  }
  if (ntriples == 1) {
    ret = TRUE
  }
  return(ret)
}

# Simulate 2 pair
cnt_2_pair = 0
cnt_3_of_kind = 0
for (j in 1:ntrials) {
  h = sample(deck, size_hand)
  if (is_two_pair(h)) {
    cnt_2_pair = cnt_2_pair + 1
  }
  if (is_three_of_a_kind(h)) {
    cnt_3_of_kind = cnt_3_of_kind + 1
  }
}
cat('Simulated prob of 2 pair =', cnt_2_pair/ntrials, '\n')
cat('Simulated prob of 3 of a kind =', cnt_3_of_kind/ntrials, '\n')

# Exact calculations for 6 card hands out of 52 card deck
# Two pair
numerator = choose(13,2)*choose(4,2)*choose(4,2)*choose(11,2)*choose(4,1)*choose(4,1)
denominator = choose(52,6)
cat('Exact two pair: numerator =', numerator, ', denominator =', denominator, ', prob 
=', numerator/denominator, '\n')

# Three of a kind
numerator = choose(13,1)*choose(4,1)*choose(12,3)*choose(4,1)*choose(4,1)*choose(4,1)
denominator = choose(52,6)
cat('Exact three of a kind: numerator =', numerator, ', denominator =', denominator, 
    ', prob =', numerator/denominator, '\n')
