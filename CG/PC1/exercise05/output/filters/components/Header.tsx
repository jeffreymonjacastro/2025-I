'use client'

import { useEffect, useState } from 'react'
import '@/styles/header.css'

export default function Header() {
  const [scrolled, setScrolled] = useState(false);

  useEffect(() => {
    const handleScroll = () => {
      if (window.scrollY > window.innerHeight) {
        setScrolled(true);
      } else {
        setScrolled(false);
      }
    };

    window.addEventListener('scroll', handleScroll);

    return () => {
      window.removeEventListener('scroll', handleScroll);
    };
  }, []);

  const scrollToSection = (id: string) => {
    const section = document.getElementById(id);
    if (section) {
      section.scrollIntoView({ behavior: 'smooth' });
    }
  };

  return (
    <nav className={`w-full p-4 text-white fixed top-0 ${scrolled ? 'scrolled' : ''}`}>
      <ul className="flex justify-evenly space-x-4">
        <li className="nav-button" onClick={() => scrollToSection('box')}>Box</li>
        <li className="nav-button" onClick={() => scrollToSection('bartlett')}>Bartlett</li>
        <li className="nav-button" onClick={() => scrollToSection('gaussian')}>Gaussian</li>
        <li className="nav-button" onClick={() => scrollToSection('laplacian')}>Laplacian</li>
      </ul>
    </nav>
  );
}
