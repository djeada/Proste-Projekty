// Rules of Yahtzee: dice, scoring, upper bonus, turns and rounds. No DOM here.

const NUM_DICE = 5;
const MAX_ROLLS = 3;
const NUM_ROUNDS = 13;
const UPPER_BONUS_LIMIT = 63;
const UPPER_BONUS = 35;

const Category = Object.freeze({
  ONES: 0,
  TWOS: 1,
  THREES: 2,
  FOURS: 3,
  FIVES: 4,
  SIXES: 5,
  THREE_OF_A_KIND: 6,
  FOUR_OF_A_KIND: 7,
  FULL_HOUSE: 8,
  SMALL_STRAIGHT: 9,
  LARGE_STRAIGHT: 10,
  YAHTZEE: 11,
  CHANCE: 12,
});

const CATEGORY_NAMES = [
  'Ones', 'Twos', 'Threes', 'Fours', 'Fives', 'Sixes',
  'Three of a Kind', 'Four of a Kind', 'Full House', 'Small Straight',
  'Large Straight', 'Yahtzee', 'Chance',
];

// counts[face] is how many dice show that face (faces are 1 to 6).
function countFaces(dice) {
  const counts = [0, 0, 0, 0, 0, 0, 0];
  for (const die of dice) {
    counts[die]++;
  }
  return counts;
}

// True when every face from start to start + length - 1 shows at least once.
function hasRun(counts, start, length) {
  return counts.slice(start, start + length).every((n) => n > 0);
}

function scoreFor(dice, category) {
  const counts = countFaces(dice);
  const total = dice.reduce((sum, die) => sum + die, 0);
  const mostOfAKind = Math.max(...counts);
  if (category <= Category.SIXES) {
    return (category + 1) * counts[category + 1];
  }
  switch (category) {
    case Category.THREE_OF_A_KIND:
      return mostOfAKind >= 3 ? total : 0;
    case Category.FOUR_OF_A_KIND:
      return mostOfAKind >= 4 ? total : 0;
    case Category.FULL_HOUSE:
      return counts.includes(3) && counts.includes(2) ? 25 : 0;
    case Category.SMALL_STRAIGHT:
      return [1, 2, 3].some((start) => hasRun(counts, start, 4)) ? 30 : 0;
    case Category.LARGE_STRAIGHT:
      return [1, 2].some((start) => hasRun(counts, start, 5)) ? 40 : 0;
    case Category.YAHTZEE:
      return mostOfAKind === NUM_DICE ? 50 : 0;
    default:
      return total;
  }
}

function upperTotal(card) {
  return card.slice(Category.ONES, Category.SIXES + 1).reduce((sum, score) => sum + (score ?? 0), 0);
}

function upperBonus(card) {
  return upperTotal(card) >= UPPER_BONUS_LIMIT ? UPPER_BONUS : 0;
}

function cardTotal(card) {
  const lower = card.slice(Category.THREE_OF_A_KIND).reduce((sum, score) => sum + (score ?? 0), 0);
  return upperTotal(card) + upperBonus(card) + lower;
}

// A game for 1 to 4 players. Each round every player takes one turn.
function newGame(numPlayers) {
  return {
    cards: Array.from({ length: numPlayers }, () => new Array(CATEGORY_NAMES.length).fill(null)),
    current: 0, // index of the player whose turn it is
    round: 1, // NUM_ROUNDS + 1 when the game is over
    dice: new Array(NUM_DICE).fill(0),
    held: new Array(NUM_DICE).fill(false),
    rolls: 0, // rolls made in this turn
  };
}

function isOver(game) {
  return game.round > NUM_ROUNDS;
}

// Rolls the dice that are not held. face() returns one value from 1 to 6.
function rollDice(game, face) {
  if (isOver(game) || game.rolls >= MAX_ROLLS) {
    return false;
  }
  game.dice = game.dice.map((die, i) => (game.held[i] ? die : face()));
  game.rolls++;
  return true;
}

// Holds or releases die 0 to 4. Needs at least one roll in this turn.
function toggleHold(game, die) {
  if (isOver(game) || game.rolls === 0) {
    return false;
  }
  game.held[die] = !game.held[die];
  return true;
}

function startNextTurn(game) {
  game.dice = new Array(NUM_DICE).fill(0);
  game.held = new Array(NUM_DICE).fill(false);
  game.rolls = 0;
  game.current++;
  if (game.current === game.cards.length) {
    game.current = 0;
    game.round++;
  }
}

function chooseCategory(game, category) {
  const card = game.cards[game.current];
  if (isOver(game) || game.rolls === 0 || card[category] !== null) {
    return false;
  }
  card[category] = scoreFor(game.dice, category);
  startNextTurn(game);
  return true;
}

// Index of the player with the highest total. Ties go to the player who comes first.
function winner(game) {
  let best = 0;
  game.cards.forEach((card, player) => {
    if (cardTotal(card) > cardTotal(game.cards[best])) {
      best = player;
    }
  });
  return best;
}

if (typeof module !== 'undefined') {
  module.exports = {
    Category, CATEGORY_NAMES, NUM_DICE, MAX_ROLLS, NUM_ROUNDS,
    scoreFor, upperTotal, upperBonus, cardTotal,
    newGame, isOver, rollDice, toggleHold, chooseCategory, winner,
  };
}
