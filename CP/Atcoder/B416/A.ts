function solveAlternative(): void {
  const input = require('fs').readFileSync('/dev/stdin', 'utf8').trim().split('\n');
  const [N, L, R] = input[0].split(' ').map(Number);
  const S = input[1];
  
  const substring = S.slice(L - 1, R);
  
  // Verificar si todos los caracteres son 'o'
  const result = substring.split('').every((char: string) => char === 'o');

  console.log(result ? 'Yes' : 'No');
}